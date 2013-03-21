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

#include "variabledescriptor.h"

bool operator==(const VariableDescriptor& a, const VariableDescriptor& b)
{

    if(a.size() != b.size())
        return false;

    for(unsigned int i = 0; i < a.size(); ++i)
        if(a[i] != b[i])
            return false;

    return true;
}

bool operator!=(const VariableDescriptor& a, const VariableDescriptor& b)
{
    return !(a==b);
}

bool operator< (const VariableDescriptor& a, const VariableDescriptor& b)
{
    for(unsigned int i = 0; i < a.size(); ++i)
    {
        if(i >= b.size())
            return false;

        if(a[i]<b[i])
            return true;

        if(a[i]>b[i])
            return false;
    }

    if(b.size() > a.size())
        return true;

    return false;
}

bool operator<=(const VariableDescriptor& a, const VariableDescriptor& b)
{
    for(unsigned int i = 0; i < a.size(); ++i)
    {
        if(i >= b.size())
            return false;

        if(a[i]<b[i])
            return true;

        if(a[i]>b[i])
            return false;
    }
    return true;
}

bool operator> (const VariableDescriptor& a, const VariableDescriptor& b)
{
    return !(a<=b);
}

bool operator>=(const VariableDescriptor& a, const VariableDescriptor& b)
{
    return !(a<b);
}


bool VariableDescriptor::inScopeOf(const VariableDescriptor &other) const
{
    for(unsigned int i = 0; i < other.size(); ++i)
    {
        if(i >= size())
            return false;

        if((*this)[i]!=other[i])
            return false;
    }
    return true;
}


VariableDescriptor::VariableDescriptor()
{
}

VariableDescriptor::VariableDescriptor(const std::vector<Variant> &other)
    : std::vector<Variant>(other)
{
}

VariableDescriptor::VariableDescriptor(std::initializer_list<Variant> l)
    : std::vector<Variant>(l)
{
}

std::ostream& operator << (std::ostream& out, const VariableDescriptor& descriptor)
{
    for(unsigned int i = 0; i < descriptor.size(); ++i)
    {
        if(i>0)
            out<<".";
        out << descriptor[i];
    }
    return out;
}
