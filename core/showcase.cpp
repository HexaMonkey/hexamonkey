#include "showcase.h"

Showcase::Showcase()
{
}

void Showcase::add(const std::string &elem)
{
    elems.push_back(elem);
}

Showcase::const_iterator Showcase::begin() const
{
    return elems.begin();
}

Showcase::const_iterator Showcase::end() const
{
    return elems.end();
}

bool Showcase::empty() const
{
    return elems.empty();
}

int Showcase::size() const
{
    return elems.size();
}
