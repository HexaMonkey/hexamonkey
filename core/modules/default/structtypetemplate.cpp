#include "structtypetemplate.h"

#include "core/modules/default/structparser.h"

#include "core/module.h"
#include "core/objecttype.h"

StructTypeTemplate::StructTypeTemplate()
    :ObjectTypeTemplate("Struct", {"_name"})
{
    setAttributeGenerator(ObjectTypeTemplate::Attribute::name,
                          []objectTypeAttributeLambda {
                                return type.parameterValue(0);
                          });
}

Parser *StructTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module &module) const
{
    auto parser = new StructParser(option, module);
    for (int i = 0, n = (type.numberOfParameters()-1)/2; i < n; ++i) {
        parser->addElement(type.parameterValue(2*i+1).toObjectType(), type.parameterValue(2*i+2).toString());
    }
    return parser;
}

int64_t StructTypeTemplate::fixedSize(const ObjectType &type, const Module &module) const
{
    int s = 0;
    for (int i = 1; i < type.numberOfParameters(); i += 2) {
        int t = module.getFixedSize(type.parameterValue(i).toObjectType());
        if (t != -1) {
            s += t;
        } else {
            return -1;
        }
    }
    return s;
}
