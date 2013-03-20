#include "tsfileparser.h"
#include "tstypes.h"

TsFileParser::TsFileParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void TsFileParser::doParse()
{
    while(availableSize()>0)
    {
        addVariable(tsTypes::junk());
        addVariable(tsTypes::transport_packet());
    }
}


bool TsFileParser::doParseSome(int hint)
{
    for(int count = 0; count < hint; ++count)
    {
        if(availableSize()<=0)
            return true;

        addVariable(tsTypes::junk());
        addVariable(tsTypes::transport_packet());
    }
    return false;
}
