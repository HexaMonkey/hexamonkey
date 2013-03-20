#ifndef BITPARSER_H
#define BITPARSER_H

#include "parser.h"

/*!
 * \brief The BitParser class
 */
class BitParser : public SimpleParser
{
public:
    BitParser(Object& object, int64_t size);
protected:
    void doParseHead();
private:
    int64_t size;
};

/*!
 * \brief The ContentParser class
 */
class ContentParser : public SimpleParser
{
public:
    ContentParser(Object& object, int64_t size);
protected:
    void doParseHead();
private:
    int64_t size;
};

#endif // BITPARSER_H
