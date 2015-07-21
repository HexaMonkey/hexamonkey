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

#include "core/util/strutil.h"
#include "core/util/osutil.h"

std::string formatDate(unsigned long secsFrom1970)
{
    time_t CTime = secsFrom1970;

    struct tm * timeinfo = gmtime (&CTime);
    char buffer [80];

    strftime (buffer,80,"%d-%m-%Y %H:%M:%S GMT",timeinfo);
    return std::string(buffer);
}

std::string formatDuration(unsigned long duration)
{
    unsigned long sec = duration % 60;
    duration /= 60;
    unsigned long min = duration % 60;
    duration /= 60;
    unsigned long hours = duration;

    std::stringstream S;


    if(hours > 0)
    {
        S<<hours<<"h";
    }
    if(hours > 0 || min > 0)
    {
        S<<min<<"m";
        S<<sec<<"s";
    }
    else
    {
        S<<sec<<"s";
    }
    return S.str();
}

std::string extension(const std::string& path)
{
    size_t n = path.rfind('.');
    if(n == std::string::npos)
    {
        return "";
    }
    else
    {
        return path.substr(n+1);
    }
}

int fromHex(char ch)
{
    if('0' <= ch && ch <= '9')
        return ch - '0';

    if('a' <= ch && ch <= 'f')
        return 10 + ch - 'a';

    if('A' <= ch && ch <= 'F')
        return 10 + ch - 'A';

    return -1;
}

uint64_t fromHex(const std::string& s)
{
    uint64_t result = 0;
    for(unsigned int i = 0 ; i < s.size() ; ++i)
    {
        result <<= 4;
        result += fromHex(s[i]);
    }
    return result;
}

std::string sizeDisplay(int64_t bitCount)
{
    std::stringstream S;
    if (bitCount > 1LL << 33)
    {
        S << std::setprecision(4) << bitCount/(double) (1LL << 33) <<" GB";
    }
    else if (bitCount > 1LL << 23)
    {
        S << std::setprecision(4) << bitCount/(double) (1LL << 23) <<" MB";
    }
    else if (bitCount > 1LL << 13)
    {
        S << std::setprecision(4) << bitCount/(double) (1LL << 13) <<" kB";
    }
    else if (bitCount % 8 == 0)
    {
        int byteCount =  bitCount/8;
        S << std::setprecision(4) << byteCount;
        if(byteCount > 1)
           S << " bytes";
        else
            S << " byte";
    }
    else
    {
        if(bitCount > 1)
            S<< bitCount<<" bits";
        else
            S<< bitCount<<" bit";
    }
    return S.str();
}


std::string defineStyle(const std::string& name)
{
    std::stringstream S;
    for(unsigned int i = 0; i < name.size(); ++i)
    {
        if('a' <= name[i] && name[i] <= 'z')
        {
            S<<(char) toupper(name[i]);
        }
        else
        {
            S<<'_'<<name[i];
        }
    }
    return S.str();
}

void output(std::ostream& /*stream*/)
{
}


std::vector<std::string> splitByChar(const std::string& string, char delim)
{
    std::vector<std::string> output;
    std::stringstream  stream;
    stream<<string<<std::string(1, delim);
    for (std::string elem; std::getline(stream,elem,delim); ) {
        output.push_back(elem);
    }

    return output;
}

std::string join(const std::vector<std::string>& input, const std::string& glue)
{
    std::stringstream result;
    bool first = true;
    for (const std::string& elem : input) {
        if (first) {
            first = false;
        } else {
            result << glue;
        }
        result << elem;
    }

    return result.str();
}
