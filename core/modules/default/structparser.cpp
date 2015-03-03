#include "structparser.h"
#include "core/module.h"

StructParser::StructParser(Object &object, const Module &module) : ContainerParser(object, module)
{
}

void StructParser::addElement(const ObjectType &type, const std::string &name)
{
    _types.push_back(type);
    _names.push_back(name);
}

void StructParser::doParseHead()
{
    // define type name
    ObjectType& type = *modifiableType();
    const Variant& nameVariable = type.parameterValue(0);
    if (nameVariable.isNull()) {
        type.setName(nameVariable.toString());
    } else {
        type.setName("{}");
    }

    int64_t s = 0;
    for (unsigned int i = 0; i < _types.size(); ++i) {
        int64_t t = module().getFixedSize(_types[i]);
        if (t > 0) {
            s += t;
        } else {
            s = -1;
        }
    }

    if (s > 0) {
        object().setSize(s);
    } else {
        s = 0;
        for (unsigned int i = 0; i < _types.size(); ++i) {
            Object* object = addVariable(_types[i], _names[i]);
            s += object->size();
        }
        object().setSize(s);
    }
}

void StructParser::doParse()
{
    if (!_parsedInHead) {
        for (unsigned int i = 0; i < _types.size(); ++i) {
            addVariable(_types[i], _names[i]);
        }
    }
}
