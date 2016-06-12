#include "ebmldatetypetemplate.h"

#include "core/object.h"

#include "core/variable/objectattributes.h"

EbmlDateTypeTemplate::EbmlDateTypeTemplate()
    : ObjectTypeTemplate("Date")
{
}

Parser *EbmlDateTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &) const
{
    Object::ParsingContext context(option);

    context.object().setSize(64);

    uint64_t integer;
    context.object().file().read(reinterpret_cast<char* >(&integer), 64);
    if(context.object().endianness() == Object::bigEndian) {
        integer = __builtin_bswap64(integer);
    }

    context.object().setValue(integer);

    context.object().attributes(true)->addNumbered().setValue(formatDate(secsFrom1970to2001+integer/1000000000LL));

    return nullptr;
}

int64_t EbmlDateTypeTemplate::fixedSize(const ObjectType &) const
{
    return 64;
}
