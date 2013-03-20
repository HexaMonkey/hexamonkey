#ifndef FLOATPARSER_H
#define FLOATPARSER_H

#include "parser.h"

class SingleFloatParser : public SimpleParser
{
public:
    SingleFloatParser(Object& object, bool bigEndian);
protected:
    void doParseHead();
private:
    bool bigEndian;
};

class DoubleFloatParser : public SimpleParser
{
public:
    DoubleFloatParser(Object& object, bool bigEndian);
protected:
    void doParseHead();
private:
    bool bigEndian;
};

class FixedFloat16Parser : public SimpleParser
{
public:
    FixedFloat16Parser(Object& object);
protected:
    void doParseHead();
};

class FixedFloat32Parser : public SimpleParser
{
public:
    FixedFloat32Parser(Object& object);
protected:
    void doParseHead();
};

#endif // FLOATPARSER_H
