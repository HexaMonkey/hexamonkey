#ifndef ENUMPARSER_H
#define ENUMPARSER_H

#include "core/containerparser.h"

class EnumParser : public ContainerParser
{
public:
    EnumParser(Object& object, const Module& module, const ObjectType& type);
    void addElement(const Variant& key, const Variant& value);

protected:
    void doParseHead();

private:
    ObjectType _enumType;
    std::vector< std::pair<Variant, Variant> > _elements;
};

#endif // ENUMPARSER_H
