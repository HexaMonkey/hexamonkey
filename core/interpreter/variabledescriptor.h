#ifndef VARIABLEDESCRIPTOR_H
#define VARIABLEDESCRIPTOR_H

#include <vector>
#include <ostream>
#include "variant.h"


class VariableDescriptor : public std::vector<Variant>
{
public:
    VariableDescriptor();
    VariableDescriptor(const std::vector<Variant>& other);
    VariableDescriptor(std::initializer_list<Variant> l);

    bool inScopeOf(const VariableDescriptor& other) const;
};


bool operator==(const VariableDescriptor& a, const VariableDescriptor& b);
bool operator!=(const VariableDescriptor& a, const VariableDescriptor& b);
bool operator< (const VariableDescriptor& a, const VariableDescriptor& b);
bool operator<=(const VariableDescriptor& a, const VariableDescriptor& b);
bool operator> (const VariableDescriptor& a, const VariableDescriptor& b);
bool operator>=(const VariableDescriptor& a, const VariableDescriptor& b);

std::ostream& operator << (std::ostream& out, const VariableDescriptor& descriptor);

#endif // VARIABLEDESCRIPTOR_H
