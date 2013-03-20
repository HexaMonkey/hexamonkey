#ifndef WORDPARSER_H
#define WORDPARSER_H

#include "parser.h"

class WordParser : public SimpleParser
{
public:
    WordParser(Object& object, int numberOfChars);
protected:
    void doParseHead();
private:
    int numberOfChars;
};

class Utf8StringParser : public SimpleParser
{
public:
    Utf8StringParser(Object& object);
protected:
    void doParseHead();
};

#endif // WORDPARSER_H
