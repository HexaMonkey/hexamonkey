#ifndef TUPLEPARSER_H
#define TUPLEPARSER_H

#include "containerparser.h"

class TupleParser : public ContainerParser
{
public:
    TupleParser(Object& object, const Module& module, const ObjectType& elementType, int64_t count);

protected:
    void doParseHead();
    void doParse();
    bool doParseSome(int hint);

private:
    ObjectType elementType;
    int64_t count;
};

#endif // TUPLEPARSER_H
