#ifndef ARRAYPARSER_H
#define ARRAYPARSER_H

#include "containerparser.h"

/*!
 * \brief The ArrayParser class
 */
class ArrayParser : public ContainerParser
{
public:
    ArrayParser(Object& object, const Module& module, const ObjectType& elementType, int64_t size);

protected:
    void doParseHead();
    void doParse();
    bool doParseSome(int hint);

private:
    ObjectType elementType;
    int64_t size;
};

#endif // ARRAYPARSER_H
