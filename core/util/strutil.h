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

template <class T>
T strTo(std::string s)
{
    std::stringstream S;
    T r;
    S<<s;
    S>>r;
    return r;
}

template <class T>
std::string toStr(T t)
{
    std::stringstream S;
    S<<t;
    return S.str();
}

template <class T>
std::string toHex(T t, int size = 0)
{
    std::stringstream S;
    S << std::setfill('0') << std::setw(size) << std::hex << t << std::dec;
    return S.str();
}

template <class T>
std::string intDisplay(T t, int base = 10)
{
    std::stringstream S;
    S << std::showbase << std::setbase(base) << t;
    return S.str();
}

int fromHex(char ch);
uint64_t fromHex(const std::string& s);

const unsigned long secsFrom1904To1970 = 2082844800L;
const unsigned long secsFrom1970to2001 = 978307200L;
std::string formatDate(unsigned long secsFrom1970);
std::string formatDuration(unsigned long duration);

std::string fixedPointRepresentation( int16_t i);
std::string fixedPointRepresentation(uint16_t i);

std::string langageRepresentation(uint16_t isoCode);
std::string fixedPointRepresentation( int32_t i);
std::string fixedPointRepresentation(uint32_t i);

std::string extension(const std::string& path);

std::string sizeDisplay(int64_t bitCount);

std::string defineStyle(const std::string& name);

#endif // STRUTIL_H_INCLUDED
