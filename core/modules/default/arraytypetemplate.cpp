#include "arraytypetemplate.h"
#include "core/objecttype.h"
#include "core/modules/default/arrayparser.h"

ArrayTypeTemplate::ArrayTypeTemplate()
    : ObjectTypeTemplate("Array",{"elementType", "size", "_namePattern"})
{
    setAttributeGenerator(ObjectTypeTemplate::Attribute::elementType,
                                           []objectTypeAttributeLambda {
                                                 return type.parameterValue(0);
                                           });
}

Parser *ArrayTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module &) const
{
    if(type.parameterSpecified(0))
    {
        const ObjectType& elemType = type.parameterValue(0).toObjectType();
        const int64_t size =
                  type.parameterSpecified(1) ? type.parameterValue(1).toInteger() : -1LL;

        const std::string& namePattern =
                  type.parameterSpecified(2) ? type.parameterValue(2).toString() : "";

        return new ArrayParser(option, elemType, size, namePattern);
    }
    return nullptr;
}

int64_t ArrayTypeTemplate::fixedSize(const ObjectType &type) const
{
    if(type.parameterSpecified(1))
    {
        int64_t s = type.parameterValue(1).toInteger();
        if(s>=0)
           return s;
    }
    return -1;
}
