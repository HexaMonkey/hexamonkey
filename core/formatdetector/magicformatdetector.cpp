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

#include "magicformatdetector.h"
#include <sstream>
#include <algorithm>
#include <iterator>

#include "strutil.h"


void MagicFormatDetector::addMagicNumber(const std::string& format, const std::string &magicNumber)
{
    _magicNumbers[format] = magicNumber;
}

void MagicFormatDetector::addMagicNumber(const std::string &format, const std::string &magicNumber, int offset)
{
    std::stringstream magicStream;
    for(int i = 0; i < offset; ++i)
    {
        magicStream<<"xx ";
    }
    magicStream<<magicNumber;
    addMagicNumber(format, magicStream.str());
}

std::string MagicFormatDetector::doGetFormat(File &file) const
{
    std::string bestFormat = "";
    unsigned int bestMagicLength = 0;
    for(const auto& entry:_magicNumbers)
    {
        const std::string& format = entry.first;
        std::stringstream magicNumber(entry.second);
        file.seekg(0, std::ios_base::beg);

        if(std::all_of(std::istream_iterator<std::string>(magicNumber) , std::istream_iterator<std::string>(), [&](const std::string& token)
        {

            if(token == "xx")
            {
                file.seekg(8, std::ios_base::cur);
                return true;
            }
            else
            {

                uint8_t byte;
                file.read(reinterpret_cast<char*>(&byte), 8);
                return (byte == fromHex(token));
            }
        }))
        {
            if(entry.second.size() > bestMagicLength)
            {
                bestFormat = format;
                bestMagicLength = entry.second.size();
            }
        }
    }
    return bestFormat;
}
