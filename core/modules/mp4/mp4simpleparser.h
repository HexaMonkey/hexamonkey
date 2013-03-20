#ifndef MP4SIMPLEPARSER_H
#define MP4SIMPLEPARSER_H

#include "parser.h"

class Mp4DateParser : public SimpleParser
{
public:
    Mp4DateParser(Object& object);
protected:
    void doParseHead();
};

class Mp4DurationParser : public SimpleParser
{
public:
    Mp4DurationParser(Object& object, int64_t scale);
protected:
    void doParseHead();
private:
    int64_t scale;
};

class Mp4LanguageParser : public SimpleParser
{
public:
    Mp4LanguageParser(Object& object);
protected:
    void doParseHead();
};
#endif // MP4SIMPLEPARSER_H
