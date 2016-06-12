#include "floattypetemplate.h"

#include "core/object.h"
#include "core/parsingexception.h"

FloatTypeTemplate::FloatTypeTemplate()
    : ObjectTypeTemplate("float")
{
}

Parser *FloatTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &) const
{
    Object::ParsingContext context(option);
    context.object().setSize(32);
    union {int32_t i; float f;} val;
    context.object().file().read(reinterpret_cast<char* >(&val.i), 32);

    if (context.object().endianness() == Object::bigEndian) {
        val.i = __builtin_bswap32(val.i);
    }

    context.object().setValue(val.f);
    return nullptr;
}

int64_t FloatTypeTemplate::fixedSize(const ObjectType &) const
{
    return 32;
}


DoubleTypeTemplate::DoubleTypeTemplate()
    : ObjectTypeTemplate("double")
{
}

Parser *DoubleTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &) const
{
    Object::ParsingContext context(option);
    context.object().setSize(64);
    union {int64_t i; double f;} val;
    context.object().file().read(reinterpret_cast<char* >(&val.i), 64);

    if (context.object().endianness() == Object::bigEndian) {
        val.i = __builtin_bswap64(val.i);
    }

    context.object().setValue(val.f);

    return nullptr;
}

int64_t DoubleTypeTemplate::fixedSize(const ObjectType &) const
{
    return 64;
}


FixedFloatTypeTemplate::FixedFloatTypeTemplate()
    : ObjectTypeTemplate("fixedFloat", {"integer","decimal"})
{

}

Parser *FixedFloatTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option, const Module &) const
{
    if (!type.parameterSpecified(0) && !type.parameterSpecified(1))
    {
        throw ParsingException(ParsingException::Type::BadParameter, "Integer size must be lower than 64");
    }

    auto integerCount = type.parameterValue(0).toInteger();
    auto decimalCount = type.parameterValue(1).toInteger();

    if (integerCount == 8 && decimalCount == 8) {
        Object::ParsingContext context(option);

        context.object().setSize(16);

        int8_t integer;
        context.object().file().read(reinterpret_cast<char* >(&integer), 8);

        uint8_t decimal;
        context.object().file().read(reinterpret_cast<char* >(&decimal), 8);

        double f = integer + decimal/pow(2,8);
        context.object().setValue(f);
    } else if (integerCount == 16 && decimalCount == 16) {
        Object::ParsingContext context(option);

        context.object().setSize(32);

        int16_t integer;
        context.object().file().read(reinterpret_cast<char* >(&integer)+1, 8);
        context.object().file().read(reinterpret_cast<char* >(&integer), 8);

        uint16_t decimal;
        context.object().file().read(reinterpret_cast<char* >(&decimal)+1, 8);
        context.object().file().read(reinterpret_cast<char* >(&decimal), 8);

        double f = integer + decimal/pow(2,16);
        context.object().setValue(f);
    } else {
        throw ParsingException(ParsingException::Type::BadParameter, "Parameters not handled");
    }

    return nullptr;
}

int64_t FixedFloatTypeTemplate::fixedSize(const ObjectType &type) const
{
    if(type.parameterSpecified(0) && type.parameterSpecified(1)) {
        return type.parameterValue(0).toInteger()+type.parameterValue(1).toInteger();
    } else {
        return unknownSize;
    }
}
