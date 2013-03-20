#include "standardmodule.h"

#include "standardtypes.h"

#include "intparser.h"
#include "bitparser.h"
#include "wordparser.h"
#include "floatparser.h"

using namespace standardTypes;

StandardModule::StandardModule(bool bigEndian)
    :bigEndian(bigEndian)
{
}

bool StandardModule::doLoad()
{
    addTemplate(integer);
    addParser("int", [this]parserLambda
    {
        if(type.parameterSpecified(0))
        {
            int size = type.parameterValue(0).toInteger();
            int base = 10;
            if(type.parameterSpecified(1))
                base = type.parameterValue(1).toInteger();
            if(size<=64)
            {
                switch(size)
                {
                    case 8:
                        return new Int8Parser(object, base);
                    case 16:
                        return new Int16Parser(object, bigEndian, base);
                    case 32:
                        return new Int32Parser(object, bigEndian, base);
                    case 64:
                        return new Int64Parser(object, bigEndian, base);
                    default:
                        return new IntXParser(object, size, bigEndian, base);
                }
            }
        }
        return nullptr;

    });
    setFixedSizeFromArg("int", 0);

    addTemplate(uinteger);
    addParser("uint", [this]parserLambda
    {
        if(type.parameterSpecified(0))
        {
            int size = type.parameterValue(0).toInteger();
            int base = 10;
            if(type.parameterSpecified(1))
                base = type.parameterValue(1).toInteger();
            if(size<=64)
            {
                switch(size)
                {
                    case 8:
                        return new UInt8Parser(object, base);
                    case 16:
                        return new UInt16Parser(object, bigEndian, base);
                    case 32:
                        return new UInt32Parser(object, bigEndian, base);
                    case 64:
                        return new UInt64Parser(object, bigEndian, base);
                    default:
                        return new UIntXParser(object, size, bigEndian, base);
                }
            }
        }
        return nullptr;
    });
    setFixedSizeFromArg("uint", 0);

    addTemplate(singleFloat);
    addParser("float", [this]parserLambda{return new SingleFloatParser(object, bigEndian);});
    setFixedSize("float", 32);

    addTemplate(doubleFloat);
    addParser("double", [this]parserLambda{return new DoubleFloatParser(object, bigEndian);});
    setFixedSize("double", 64);

    addTemplate(fixedFloat);
    addParser("fixedFloat", []parserLambda
    {
        if(type == fixed16)
            return new FixedFloat16Parser(object);
        if(type == fixed32)
            return new FixedFloat32Parser(object);
        return nullptr;
    });
    setFixedSize("fixedFloat", []fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
             return type.parameterValue(0).toInteger()+type.parameterValue(1).toInteger();
         return -1;
    });

    addTemplate(string);
    addParser("String", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new WordParser(object, type.parameterValue(0).toInteger());
        return new Utf8StringParser(object);
    });
    setFixedSize("String", []fixedSizeLambda
    {
         if(type.parameterSpecified(0))
             return 8*type.parameterValue(0).toInteger();
         return -1;
    });


    addTemplate(bitset);
    addParser("Bitset", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new BitParser(object, type.parameterValue(0).toInteger());
        return nullptr;
    });
    setFixedSizeFromArg("Bitset", 0);

    addTemplate(data);
    addParser("Data", []parserLambda
    {
        if(type.parameterSpecified(0))
            return new ContentParser(object, type.parameterValue(0).toInteger());
        else
            return new ContentParser(object, -1);
    });
    setFixedSizeFromArg("Data", 0);
#if 0
    addTemplate(pair);
    addParser("Pair", []parserLambda
    {
        if(type.parameterSpecified(0) && type.parameterSpecified(1))
        {
            if(type.parameterSpecified(2) && type.parameterSpecified(3))
                return new PairParser(object, module, type.parameterValue(0).toObjectType(), type.parameterValue(1).toObjectType(), type.parameterValue(2).toString(), type.parameterValue(3).toString());
            else
                return new PairParser(object, module, type.parameterValue(0).toObjectType(), type.parameterValue(1).toObjectType());
        }
        return nullptr;
    });
    setFixedSize("Pair", [this]fixedSizeLambda
    {
         if(type.parameterSpecified(0) && type.parameterSpecified(1))
         {
             int64_t t0 = getFixedSize(type.parameterValue(0).toObjectType());
             int64_t t1 = getFixedSize(type.parameterValue(1).toObjectType());
             if(t0>0 && t1>0)
                return t0 + t1;
         }
         return -1;
    });
#endif
    return true;
}
