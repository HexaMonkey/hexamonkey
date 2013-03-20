#ifndef EBMLSIMPLEPARSER_H
#define EBMLSIMPLEPARSER_H

#include "containerparser.h"

class EbmlIntegerParser : public ContainerParser
{
public:
    EbmlIntegerParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

class EbmlUIntegerParser : public ContainerParser
{
public:
    EbmlUIntegerParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

class EbmlStringParser : public ContainerParser
{
public:
    EbmlStringParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

class EbmlBinaryParser : public ContainerParser
{
public:
    EbmlBinaryParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

class EbmlDateContainerParser : public ContainerParser
{
public:
    EbmlDateContainerParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

class EbmlFloatParser : public ContainerParser
{
public:
    EbmlFloatParser(Object& object, const Module& module);

protected:
    void doParseHead();
};

#endif // EBMLSIMPLEPARSER_H
