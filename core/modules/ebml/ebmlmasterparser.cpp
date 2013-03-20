#include "ebmlmasterparser.h"

#include "module.h"
#include "objecttypetemplate.h"

EbmlMasterParser::EbmlMasterParser(Object& object, const Module& module)
    :ContainerParser(object, module)
{
}

void EbmlMasterParser::doParse()
{
    while(availableSize()>0)
    {
        addVariable(module().getTemplate("EBMLElement")());
    }
}

bool EbmlMasterParser::doParseSome(int hint)
{
    int count = 0;
    while(true)
    {
        if(availableSize()<=0)
            return true;

        if(count>=hint)
            return false;

        addVariable(module().getTemplate("EBMLElement")());
        ++count;
    }
}
