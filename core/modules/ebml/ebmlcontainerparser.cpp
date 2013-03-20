#include "ebmlcontainerparser.h"
#include "ebmltypes.h"

EbmlContainerParser::EbmlContainerParser(Object &object, const Module &module)
    : ContainerParser(object, module)
{
}

void EbmlContainerParser::doParseHead()
{
    Object* p_id = addVariable(ebmlTypes::largeInteger(), "id");
    Object* p_size = addVariable(ebmlTypes::largeInteger(), "size");

    setSize(8*p_size->value().toInteger()+p_id->size()+p_size->size());

    type().setParameter(0, p_id->value());
}


