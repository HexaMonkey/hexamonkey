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

#ifndef STRUTIL_H_INCLUDED
#define STRUTIL_H_INCLUDED

#include <ctime>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <stdint.h>

/**
 * @brief Convert a string into another type using standard stream conversion
 */
template <class T>
T strTo(std::string s)
{
    std::stringstream S;
    T r;
    S<<s;
    S>>r;
    return r;
}

/**
 * @brief Convert a object into a string using standard stream conversion
 */
template <class T>
std::string toStr(T t)
{
    std::stringstream S;
    S<<t;
    return S.str();
}

void output(std::ostream& stream);


template <typename T, typename ...P>
void output(std::ostream& stream, const T& t,const P&... p)
{
    stream << t;
    if (!sizeof...(P))
        return;

    output(stream, p...);
}

/** @brief Concatenate random values using stringstream */
template <typename T, typename ...P>
std::string concat(T t, P ...p)
{
    std::stringstream s;
    output(s, t, p...);
    return s.str();
}

/**
 * Get an hex representation of a number
 */
template <class T>
std::string toHex(T t, int size = 0)
{
    std::stringstream S;
    S << std::setfill('0') << std::setw(size) << std::hex << t << std::dec;
    return S.str();
}

/**
 * Get a representation of a number
 */
template <class T>
std::string intDisplay(T t, int base = 10)
{
    std::stringstream S;
    S << std::showbase << std::setbase(base) << t;
    return S.str();
}

/**
 * @brief Parse a number from an hex digit
 */
int fromHex(char ch);

/**
 * @brief Parse a number from an hex string
 */
uint64_t fromHex(const std::string& s);

const unsigned long secsFrom1904To1970 = 2082844800L;
const unsigned long secsFrom1970to2001 = 978307200L;

/**
 * @brief Format a date as "{day}-{month}-{year} {hour}:{minute}:{second} GMT"
 */
std::string formatDate(unsigned long secsFrom1970);

/**
 * @brief Format a duration as "{hour}h{minutes}m{second}s"
 */
std::string formatDuration(unsigned long duration);

/**
 * @brief Get the extension of a file name
 */
std::string extension(const std::string& path);

/**
 * @brief Get a representation of a file size
 */
std::string sizeDisplay(int64_t bitCount);

/**
 * @brief camelCase to DEFINE_STYLE
 */
std::string defineStyle(const std::string& name);

#endif // STRUTIL_H_INCLUDED
