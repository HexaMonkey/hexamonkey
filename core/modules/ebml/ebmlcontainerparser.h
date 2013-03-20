#ifndef EBMLCONTAINERPARSER_H
#define EBMLCONTAINERPARSER_H

#include "containerparser.h"

class EbmlContainerParser : public ContainerParser
{
public:
    EbmlContainerParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

#endif // EBMLCONTAINERPARSER_H
