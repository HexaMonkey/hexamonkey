#include "tsjunkparser.h"
#include "standardtypes.h"

using namespace standardTypes;

TsJunkParser::TsJunkParser(Object &object, const Module &module)
    :ContainerParser(object, module)
{
    setAutogrow();
}

void TsJunkParser::doParseHead()
{
    while(true)
    {
        Object* syncbyte = getVariable(uint8);

        if(syncbyte->value().toUnsignedInteger() == 0x47)
        {
            syncbyte->seekBeginning();
            break;
        }
        else
        {
            std::cout<<syncbyte->value().toUnsignedInteger()<<std::endl;
            addChild(syncbyte);
            syncbyte->seekEnd();
        }
    }
}
