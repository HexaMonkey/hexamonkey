#include "ebmlelementtypetemplate.h"

#include "core/object.h"

#include "core/module.h"

EbmlElementTypeTemplate::EbmlElementTypeTemplate()
    : ObjectTypeTemplate("EBMLElement", {"id"})
{
    setVirtual(true);
}

Parser *EbmlElementTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);

    const auto largeInteger = module.getType("LargeInteger");
    Object* p_id = context.object().addVariable(largeInteger, "id");
    Object* p_size = context.object().addVariable(largeInteger, "size");

    context.object().setSize(p_id->size() + p_size->size() + 8 * p_size->value().toInteger());

    context.object().type().setParameter(0, p_id->value());

    return nullptr;
}
