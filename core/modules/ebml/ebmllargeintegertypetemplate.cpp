#include "ebmllargeintegertypetemplate.h"
#include "core/object.h"

EbmlLargeIntegerTypeTemplate::EbmlLargeIntegerTypeTemplate()
    :ObjectTypeTemplate("LargeInteger")
{
}

Parser *EbmlLargeIntegerTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);

    uint8_t byte;
    context.object().file().read(reinterpret_cast<char*>(&byte), 8);
    int count;
    for(count = 1; count <= 8; ++count)
    {
        if(byte&(1<<(8-count)))
            break;
    }
    int64_t var = byte & ~(1<<(8-count));

    for(int i = 1; i < count; ++i)
    {
        uint8_t byte;
        context.object().file().read(reinterpret_cast<char*>(&byte), 8);
        var = var<<8 | byte;
    }
    context.object().setSize(8*count);
    context.object().setValue(var);

    return nullptr;
}
