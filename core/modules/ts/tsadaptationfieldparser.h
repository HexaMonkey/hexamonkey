#ifndef TSADAPTATIONFIELDPARSER_H
#define TSADAPTATIONFIELDPARSER_H

#include "containerparser.h"

/*!
 * \brief The TsAdaptationFieldParser class
 */
class TsAdaptationFieldParser : public ContainerParser
{
public:
    TsAdaptationFieldParser(Object& object, const Module& module);

protected:
    void doParseHead();
    void doParse();

};

#endif // TSADAPTATIONFIELDPARSER_H
