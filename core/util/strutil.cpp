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

#include "strutil.h"


std::string formatDate(unsigned long secsFrom1970)
{
    #ifdef __linux
    struct tm MacTime = {0,0,0,0,0,0,0,0,0,0,0};
    #else
    struct tm MacTime = {0,0,0,0,0,0,0,0,0};
    #endif

    time_t CTime;

    /* Create unnormalized struct tm*/
    MacTime.tm_sec = secsFrom1970;
    MacTime.tm_year = 70;

    /* Convert to time_t */
    CTime = mktime(&MacTime);

    struct tm * timeinfo;
    char buffer [80];

    timeinfo = localtime ( &CTime );

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

std::string fixedPointRepresentation( int16_t i)
{
    double d = i;
    d /= pow(2.0,8);
    return toStr(d);
}

std::string fixedPointRepresentation(uint16_t i)
{
    double d = i;
    d /= pow(2.0,8);
    return toStr(d);
}

std::string fixedPointRepresentation( int32_t i)
{
    double d = i;
    d /= pow(2.0,16);
    return toStr(d);
}

std::string fixedPointRepresentation(uint32_t i)
{
    double d = i;
    d /= pow(2.0,16);
    return toStr(d);
}

std::string langageRepresentation(uint16_t isoCode)
{
    std::stringstream S;
    for(unsigned int i = 0; i < 3; ++i)
    {
        uint16_t letter = isoCode;
        letter<<=1+i*5;
        letter>>=11;
        S<<(char) (letter+0x60);
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
