#include "stringtypetemplate.h"

#include "core/object.h"

StringTypeTemplate::StringTypeTemplate()
    : ObjectTypeTemplate("String", {"charCount"})
{
}

Parser *StringTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option) const
{
    Object::ParsingContext context(option);

    auto numberOfChars = -1;
    if (type.parameterSpecified(0))
    {
        numberOfChars = type.parameterValue(0).toInteger();
    }

    std::stringstream S;
    std::streamoff stringLength = 0;
    while((numberOfChars == -1 || stringLength < numberOfChars) && context.object().file().good())
    {
        char ch;
        context.object().file().read(&ch, 8);

        ++stringLength;
        if(ch == '\0')
        {
            break;
        }
        unsigned char mask = 1 << 7;
        if((ch & mask) == 0)
        {
            S<<ch;
        }
        else
        {
            S <<'?';
            unsigned char& toCount = reinterpret_cast<unsigned char&>(ch);
            char testChar = 1;
            //skipping extended characters but still
            //check if no \0, to avoid infinite loop on faulty
            //data
            for(mask>>=1; toCount & mask; mask>>=1)
            {
                context.object().file().read(&testChar, 8);
                ++stringLength;
                if (testChar == '\0') {
                    break;
                }
            }

            if (testChar == '\0') {
                break;
            }
        }
    }
    const std::string& value = S.str();
    context.object().setValue(value);
    if (numberOfChars == -1) {
        context.object().setSize(8 * stringLength);
    } else {
        context.object().setSize(8 * numberOfChars);
    }

    return nullptr;
}

int64_t StringTypeTemplate::fixedSize(const ObjectType &type) const
{
    if (type.parameterSpecified(0))
    {
        return 8*type.parameterValue(0).toInteger();
    } else {
        return unknownSize;
    }
}


WStringTypeTemplate::WStringTypeTemplate()
    : ObjectTypeTemplate("WString", {"charCount"})
{
}

Parser *WStringTypeTemplate::parseOrGetParser(const ObjectType &type, ParsingOption &option) const
{
    Object::ParsingContext context(option);

    auto numberOfChars = -1;
    if (type.parameterSpecified(0))
    {
        numberOfChars = type.parameterValue(0).toInteger();
    }

    std::stringstream S;
    std::streamoff stringLength = 0;

    for(int i = 0; numberOfChars == -1 || i < numberOfChars; ++i)
    {
        uint16_t ch = 0;
        if(context.object().endianness() == Object::bigEndian) {
            context.object().file().read(reinterpret_cast<char*>(&ch)+1, 8);
            context.object().file().read(reinterpret_cast<char*>(&ch), 8);
        } else {
            context.object().file().read(reinterpret_cast<char*>(&ch), 16);
        }
        ++stringLength;

        if(ch == 0) {
            break;
        }

        if(ch < 0x80) {
            S << (char)ch;
        } else {
            S << '?';
        }
    }

    if (numberOfChars == -1) {
        context.object().setSize(16 * stringLength);
    }  else {
        context.object().setSize(16 * numberOfChars);
    }

    context.object().setValue(S.str());

    return nullptr;
}

int64_t WStringTypeTemplate::fixedSize(const ObjectType &type) const
{
    if (type.parameterSpecified(0))
    {
        return 16*type.parameterValue(0).toInteger();
    } else {
        return unknownSize;
    }
}
