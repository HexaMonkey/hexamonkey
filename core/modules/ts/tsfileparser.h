#ifndef TSFILEPARSER_H
#define TSFILEPARSER_H

#include "containerparser.h"

/*!
 * \brief The TsFileParser class
 */
class TsFileParser : public ContainerParser
{
public:
    TsFileParser(Object& object, const Module& module);

protected:
    void doParse();
    bool doParseSome(int hint);
};

#endif // TSFILEPARSER_H
