#ifndef EBMLDATEPARSER_H
#define EBMLDATEPARSER_H

#include "parser.h"

class EbmlDateParser : public SimpleParser
{
public:
    EbmlDateParser(Object& object);

protected:
    void doParseHead();
};

#endif // EBMLDATEPARSER_H
