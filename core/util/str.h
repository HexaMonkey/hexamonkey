#include <sstream>
#include <iostream>

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
