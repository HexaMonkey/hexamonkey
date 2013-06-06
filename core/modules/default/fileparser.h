#ifndef FILEPARSER_H
#define FILEPARSER_H

#include "parser.h"

/*!
 * @brief The FileParser class
 */
class FileParser : public SimpleParser
{
public:
    FileParser(Object& object);

protected:
    void doParseHead();
};

#endif // FILEPARSER_H
