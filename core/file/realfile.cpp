//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include "core/file/realfile.h"
#include "core/formatdetector/formatdetector.h"
#include "core/util/strutil.h"
#include "core/util/bitutil.h"
#include "core/log/logmanager.h"

RealFile::RealFile() : File()
{
}

void RealFile::setPath(const std::string& path)
{
    _path = path;
    open();
}

const std::string& RealFile::path() const
{
    return _path;
}

void RealFile::open()
{
    _file.open(_path.c_str(), std::ios::in|std::ios::binary);
    if(!good()) {
        Log::error("Unable to open file", _path);
    } else {
        FileAnchor fileAnchor(*this);
        _file.seekg(0, std::ios::end);
        _size = tellg();
    }
}

void RealFile::close()
{
    _file.close();
}

void RealFile::clear()
{
    _file.clear();
}

void RealFile::read(char* s, int64_t count )
{
    if(count == 0)
        return;
    if (_bitPosition == 0 && count % 8 == 0)
        _file.read(s,count/8);
    else
    {
        int64_t bitCount = (count + _bitPosition);
        int64_t byteCount = (bitCount&7)? bitCount/8 + 1 : bitCount/8;
        int8_t shift = byteCount*8 - bitCount;
        uint8_t mask = lsbMask(8-_bitPosition);

        uint8_t buffer[byteCount];
        _file.read(reinterpret_cast<char*>(buffer),byteCount);

        //Delete first bits
        buffer[0] &= mask;

        //Shift bits
        buffer[byteCount-1] >>= shift;

        for(int i = byteCount-2; i>=0; i--)
        {
            buffer[i+1] |= buffer[i] << (8-shift) ;
            buffer[i] >>= shift;
        }

        //Copy bits
        int firstByte = (_bitPosition+shift>=8)? 1 : 0;
        for(int i = firstByte; i < byteCount; ++i)
        {
            s[i-firstByte] = buffer[i];
        }

        //Set new file position
        if ((bitCount & 0x7) != 0)
        {
            _file.seekg(-1,std::ios_base::cur);
        }
        _bitPosition = (count + _bitPosition)  & 0x7;
    }
}


void RealFile::seekg(int64_t off, std::ios_base::seekdir dir) {
    switch (dir)
    {
        case std::ios_base::beg :
            _bitPosition = off & 0x7; // & 0x7 stands for % 8
        break;
        case std::ios_base::end :
            _bitPosition =  off & 0x7;
        break;
        default:
            _bitPosition = (_bitPosition + off) & 0x7;
            off += _bitPosition;
        break;
    }
    _file.seekg(off / 8, dir);
}


int64_t RealFile::tellg() {return _file.tellg() * 8 + _bitPosition;}


int64_t RealFile::size()
{
    return _size;
}

bool RealFile::good()
{
    return _file.is_open()&&!_file.fail()&&!_file.bad()&&!_file.eof();
}
