#ifndef INTPARSER_H
#define INTPARSER_H

#include "parser.h"

class Int8Parser : public SimpleParser
{
public:
    Int8Parser(Object& object, int base = 10);
protected:
    void doParseHead();
	int base;
};

class Int16Parser : public SimpleParser
{
public:
    Int16Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class Int32Parser : public SimpleParser
{
public:
    Int32Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class Int64Parser : public SimpleParser
{
public:
    Int64Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class IntXParser : public SimpleParser
{
public:
    IntXParser(Object& object, uint8_t size, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    uint8_t size;
    bool bigEndian;
    int base;
};

class UInt8Parser : public SimpleParser
{
public:
    UInt8Parser(Object& object, int base = 10);
protected:
    void doParseHead();
	int base;
};

class UInt16Parser : public SimpleParser
{
public:
    UInt16Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class UInt32Parser : public SimpleParser
{
public:
    UInt32Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class UInt64Parser : public SimpleParser
{
public:
    UInt64Parser(Object& object, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    bool bigEndian;
    int base;
};

class UIntXParser : public SimpleParser
{
public:
    UIntXParser(Object& object, uint8_t size, bool bigEndian, int base = 10);
protected:
    void doParseHead();
private:
    uint8_t size;
    bool bigEndian;
    int base;
};

#endif // INTPARSER_H
