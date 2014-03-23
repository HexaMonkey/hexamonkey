
#ifndef PARENT_PID_PARSER_H
#define PARENT_PID_PARSER_H

#include "core/parser.h"

class ParentPidParser : public SimpleParser
{
public:
    ParentPidParser(Object& object);
protected:
    void doParseHead();
};

#endif // PARENT_PID_PARSER_H