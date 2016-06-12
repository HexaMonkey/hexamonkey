#include "datatypetemplate.h"

#include "core/object.h"

DataTypeTemplate::DataTypeTemplate()
    :ObjectTypeTemplate("Data", {"_size"})
{
}

Parser *DataTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module &) const
{
    Object::ParsingContext context(option);

    if (type.parameterSpecified(0))
    {
        auto size =  type.parameterValue(0).toInteger();
        if (size != unknownSize) {
            context.object().setSize(type.parameterValue(0).toInteger());
        } else {
            context.object().setToExpandOnAddition();
        }
    } else {
        context.object().setToExpandOnAddition();
    }
    return nullptr;
}

int64_t DataTypeTemplate::fixedSize(const ObjectType &type) const
{
    if (type.parameterSpecified(0))
    {
        return type.parameterValue(0).toInteger();
    } else {
        return unknownSize;
    }
}


