#include "ebmlsimpleparser.h"

#include "standardtypes.h"
#include "ebmltypes.h"

EbmlIntegerParser::EbmlIntegerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlIntegerParser::doParseHead()
{
    addVariable(standardTypes::integer(availableSize()),"payload");
    showcase().add("payload");
}

EbmlUIntegerParser::EbmlUIntegerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlUIntegerParser::doParseHead()
{
    addVariable(standardTypes::uinteger(availableSize()),"payload");
    showcase().add("payload");
}

EbmlStringParser::EbmlStringParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlStringParser::doParseHead()
{
    addVariable(standardTypes::string(availableSize()/8),"payload");
    showcase().add("payload");
}

EbmlBinaryParser::EbmlBinaryParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlBinaryParser::doParseHead()
{
    addVariable(standardTypes::data(availableSize()), "payload");
}

EbmlDateContainerParser::EbmlDateContainerParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlDateContainerParser::doParseHead()
{
    addVariable(ebmlTypes::Date(), "payload");
    showcase().add("payload");
}

EbmlFloatParser::EbmlFloatParser(Object& object, const Module& module)
    : ContainerParser(object, module)
{
}

void EbmlFloatParser::doParseHead()
{
    if(availableSize() == 32)
        addVariable(standardTypes::singleFloat(),"payload");
    else
        addVariable(standardTypes::doubleFloat(),"payload");
    showcase().add("payload");
}
