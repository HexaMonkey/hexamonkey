#ifndef DATAPARSER_H
#define DATAPARSER_H

#include "core/parser.h"

class DataParser : public SimpleParser
{
public:
    DataParser(Object& object, int64_t size);

protected:
    void doParseHead();
private:
    int64_t size;
};

#endif // DATAPARSER_H
