#include "mp4simpleparser.h"

Mp4DateParser::Mp4DateParser(Object &object)
    : SimpleParser(object)
{
}

void Mp4DateParser::doParseHead()
{
    setInfo("\""+formatDate(object().value().toUnsignedInteger()-secsFrom1904To1970)+"\"");
}


Mp4DurationParser::Mp4DurationParser(Object &object, int64_t scale)
    : SimpleParser(object), scale(scale)
{
}

void Mp4DurationParser::doParseHead()
{
    setInfo("\""+formatDuration(object().value().toInteger()/scale)+"\"");
}


Mp4LanguageParser::Mp4LanguageParser(Object &object)
    :SimpleParser(object)
{
}

void Mp4LanguageParser::doParseHead()
{
    setInfo("\""+langageRepresentation(object().value().toInteger())+"\"");
}
