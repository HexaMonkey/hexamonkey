#ifndef EBMLLARGEINTEGERPARSER_H
#define EBMLLARGEINTEGERPARSER_H

#include "parser.h"

class EbmlLargeIntegerParser : public SimpleParser
{
public:
    EbmlLargeIntegerParser(Object& object);

protected:
    void doParseHead();
};

#endif // EBMLLARGEINTEGERPARSER_H
