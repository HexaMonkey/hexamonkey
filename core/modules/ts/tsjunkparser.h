#ifndef TSJUNKPARSER_H
#define TSJUNKPARSER_H

#include "containerparser.h"

/*!
 * \brief The TsJunkParser class
 */
class TsJunkParser : public ContainerParser
{
public:
    TsJunkParser(Object& object, const Module& module);
protected:
    void doParseHead();
};

#endif // TSJUNKPARSER_H
