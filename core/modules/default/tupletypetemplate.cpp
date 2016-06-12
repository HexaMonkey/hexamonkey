#include "tupletypetemplate.h"
#include "core/objecttype.h"
#include "core/module.h"
#include "core/modules/default/tupleparser.h"

TupleTypeTemplate::TupleTypeTemplate()
    : ObjectTypeTemplate("Tuple",{"elementType", "count", "_namePattern"})
{
    setAttributeGenerator(ObjectTypeTemplate::Attribute::elementType,
                                           []objectTypeAttributeLambda {
                                                 return type.parameterValue(0);
                                           });

    setAttributeGenerator(ObjectTypeTemplate::Attribute::elementCount,
                                           []objectTypeAttributeLambda {
                                                 return type.parameterValue(1);
                                           });
}

Parser *TupleTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module &module) const
{
    if(type.parameterSpecified(0) && type.parameterSpecified(1))
    {
        const ObjectType& elemType = type.parameterValue(0).toObjectType();
        const int64_t elemCount = type.parameterValue(1).toInteger();
        const std::string& namePattern =
                   type.parameterSpecified(2) ? type.parameterValue(2).toString() : "";
        return new TupleParser(option, module, elemType, elemCount, namePattern);
    }
    return nullptr;
}

int64_t TupleTypeTemplate::fixedSize(const ObjectType &type) const
{
    if(type.parameterSpecified(0) && type.parameterSpecified(1))
    {
        int64_t t = type.parameterValue(0).toObjectType().fixedSize();
        if(t>=0)
           return t*type.parameterValue(1).toInteger();
    }
    return -1;
}
