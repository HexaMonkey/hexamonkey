#ifndef EBMLMASTERPARSER_H
#define EBMLMASTERPARSER_H

#include "containerparser.h"

class EbmlMasterParser : public ContainerParser
{
public:
    EbmlMasterParser(Object& object, const Module& module);

protected:
    void doParse();
    bool doParseSome(int hint);
};

#endif // EBMLMASTERPARSER_H
