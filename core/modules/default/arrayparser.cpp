#include "arrayparser.h"

ArrayParser::ArrayParser(Object &object, const Module &module, const ObjectType &elementType, int64_t size)
    :ContainerParser(object, module), elementType(elementType), size(size)
{
    setExpandOnAddition();
}

void ArrayParser::doParseHead()
{
    setSize(size);
}

void ArrayParser::doParse()
{
    while(availableSize())
    {
        Object* object = addVariable(elementType);
        object->setName("@"+toStr(object->rank()));
    }
}

bool ArrayParser::doParseSome(int hint)
{
    for(int count = 0; count < hint; ++count)
    {
        if(availableSize()<=0)
            return true;
        Object* object = addVariable(elementType);
        object->setName("@"+toStr(object->rank()));
    }
    return false;
}
