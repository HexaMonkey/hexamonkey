#ifndef TSPACKETPARSER_H
#define TSPACKETPARSER_H

#include "containerparser.h"

/*!
 * \brief The TsPacketParser class
 */
class TsPacketParser : public ContainerParser
{
public:
    TsPacketParser(Object& object, const Module& module);

protected:
    void doParseHead();
    void doParse();
};

#endif // TSPACKETPARSER_H
