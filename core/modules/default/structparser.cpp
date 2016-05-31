#include "structparser.h"
#include "core/module.h"

StructParser::StructParser(ParsingOption &option, const Module &module)
    : ContainerParser(option, module)
{
}

void StructParser::addElement(const ObjectType &type, const std::string &name)
{
    _types.push_back(type);
    _names.push_back(name);
}

void StructParser::doParseHead()
{
    int64_t s = 0;
    for (unsigned int i = 0; i < _types.size(); ++i) {
        int64_t t = _types[i].fixedSize(module());
        if (t >= 0) {
            s += t;
        } else {
            s = -1;
            break;
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
        _parsedInHead = true;
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
