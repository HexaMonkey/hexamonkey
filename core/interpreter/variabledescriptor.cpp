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
