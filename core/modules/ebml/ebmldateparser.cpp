#include "ebmldateparser.h"

EbmlDateParser::EbmlDateParser(Object &object)
    : SimpleParser(object)
{
}

void EbmlDateParser::doParseHead()
{
    setInfo("\""+formatDate(secsFrom1970to2001+object().value().toInteger()/1000000000LL)+"\"");
}
