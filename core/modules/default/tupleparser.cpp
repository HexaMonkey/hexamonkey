#include "tupleparser.h"

#include "module.h"

TupleParser::TupleParser(Object &object, const Module &module, const ObjectType &elementType, int64_t count)
    :  ContainerParser(object, module), elementType(elementType), count(count)
{
}

void TupleParser::doParseHead()
{
    int64_t t = module().getFixedSize(elementType);
    if(t > 0)
    {
        setSize(count*t);
    }
    else
    {
        int64_t s = 0;
        for(int64_t i = 0; i < count; ++i)
        {
            Object* object = addVariable(elementType);
            object->setName("@"+toStr(object->rank()));
            s+=object->size();
        }
        setSize(s);
    }
}

void TupleParser::doParse()
{
    while(availableSize())
    {
        Object* object = addVariable(elementType);
        object->setName("@"+toStr(object->rank()));
    }
}

bool TupleParser::doParseSome(int hint)
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
