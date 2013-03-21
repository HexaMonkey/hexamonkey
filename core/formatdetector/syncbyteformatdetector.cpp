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

#include "syncbyteformatdetector.h"
#include <algorithm>

void SyncbyteFormatDetector::addSyncbyte(const std::string& format, uint8_t syncbyte, int packetlength)
{
    _formats[format] = std::make_pair(syncbyte, packetlength);
}

std::string SyncbyteFormatDetector::doGetFormat(File &file) const
{
    for(const auto& entry:_formats)
    {
        file.seekg(0, std::ios_base::beg);
        std::string format = entry.first;
        uint8_t syncbyte = entry.second.first;
        int packetlength = entry.second.second;

        std::vector<int> occurences(packetlength,0);
        for(int i = 0; i < check; ++i)
        {
            for(int j = 0; j < packetlength; ++j)
            {
                uint8_t byte;
                file.read(reinterpret_cast<char*>(&byte), 8);
                if(byte == syncbyte)
                    occurences[j]++;
            }
        }

        if(std::any_of(occurences.begin(), occurences.end(), [](int i){return i==check;}))
        {
            return format;
        }
    }
    return "";
}
