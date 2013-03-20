#include "floatparser.h"

SingleFloatParser::SingleFloatParser(Object &object, bool bigEndian)
    : SimpleParser(object), bigEndian(bigEndian)
{
}

void SingleFloatParser::doParseHead()
{
    setSize(32);
    union {int32_t i; float f;} val;
    file().read(reinterpret_cast<char* >(&val.i), 32);
    if(bigEndian)
        val.i = __builtin_bswap32(val.i);

    setValue(val.f);
    setInfo(toStr(val.f));
}


DoubleFloatParser::DoubleFloatParser(Object &object, bool bigEndian)
    : SimpleParser(object), bigEndian(bigEndian)
{
}

void DoubleFloatParser::doParseHead()
{
    setSize(64);
    union {int64_t i; double f;} val;
    file().read(reinterpret_cast<char* >(&val.i), 64);
    if(bigEndian)
        val.i = __builtin_bswap64(val.i);

    setValue(val.f);
    setInfo(toStr(val.f));
}


FixedFloat16Parser::FixedFloat16Parser(Object &object) : SimpleParser(object)
{
}

void FixedFloat16Parser::doParseHead()
{
    setSize(16);

    int8_t integer;
    file().read(reinterpret_cast<char* >(&integer), 8);

    uint8_t decimal;
    file().read(reinterpret_cast<char* >(&decimal), 8);

    double f = integer + decimal/pow(2,8);
    setValue(f);
    setInfo(toStr(f));
}


FixedFloat32Parser::FixedFloat32Parser(Object &object) : SimpleParser(object)
{
}

void FixedFloat32Parser::doParseHead()
{
    setSize(32);

    int16_t integer;
    file().read(reinterpret_cast<char* >(&integer)+1, 8);
    file().read(reinterpret_cast<char* >(&integer), 8);

    uint16_t decimal;
    file().read(reinterpret_cast<char* >(&decimal)+1, 8);
    file().read(reinterpret_cast<char* >(&decimal), 8);

    double f = integer + decimal/pow(2,16);
    setValue(f);
    setInfo(toStr(f));
}
