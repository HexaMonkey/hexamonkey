#include "integertypetemplate.h"
#include "core/object.h"
#include "core/parsingexception.h"

IntegerTypeTemplate::IntegerTypeTemplate()
    : ObjectTypeTemplate("int",{"size", "_base"})
{
}

Parser *IntegerTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option) const
{
    Object::ParsingContext context(option);

    const int64_t size =  type.parameterValue(0).toInteger();
    Object& object = context.object();

    object.setSize(size);

    Variant value;

    switch (size) {
    case 8:
    {
        int8_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 8);
        value.setValue(integer);
        break;
    }

    case 16:
    {
        int16_t integer;
        if(object.endianness() == Object::bigEndian)
        {
            object.file().read(reinterpret_cast<char* >(&integer)+1, 8);
            object.file().read(reinterpret_cast<char* >(&integer), 8);
        } else {
            object.file().read(reinterpret_cast<char* >(&integer), 16);
        }
        value.setValue(integer);
        break;
    }

    case 32:
    {
        int32_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 32);
        if(object.endianness() == Object::bigEndian) {
            integer = __builtin_bswap32(integer);
        }

        value.setValue(integer);
        break;
    }

    case 64:
    {
        int64_t integer;
        object.file().read(reinterpret_cast<char* >(&integer), 64);
        if(object.endianness() == Object::bigEndian) {
            integer = __builtin_bswap64(integer);
        }

        value.setValue(integer);
        break;
    }

    default:
        if(size>64)
        {
            throw ParsingException(ParsingException::Type::BadParameter, "Integer size must be lower than 64");
        }
        {
            int byteSize = (size + 7) >> 3;
            char buffer[byteSize];
            int64_t integer = 0;
            char* pInteger = reinterpret_cast<char* >(&integer);
            object.file().read(buffer, size);
            for (int i = 0 ; i < byteSize ; i++) {
                if(object.endianness() == Object::bigEndian)
                    pInteger[byteSize - 1 - i] = buffer[i];
                else
                    pInteger[i] = buffer[i];
            }

            if (integer & 1LL<<(size-1) ) {
                integer |= 0xFFFFFFFFFFFFFFFFLL << size;
            }

            value.setValue(integer);
        }
        break;
    }



    int base = 0;
    if (type.parameterSpecified(1)) {
        base = type.parameterValue(1).toInteger();
    }
    switch (base) {
        case 2:
            value.setDisplayBase(Variant::binary);
            break;

        case 8:
            value.setDisplayBase(Variant::octal);
            break;

        case 16:
            value.setDisplayBase(Variant::hexadecimal);
            break;

        default:
            value.setDisplayBase(Variant::decimal);
    }

    object.setValue(value);

    return nullptr;
}

int64_t IntegerTypeTemplate::fixedSize(const ObjectType &objectType) const
{
    return objectType.parameterValue(0).toInteger();
}
