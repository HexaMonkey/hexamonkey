#include "ebmlelementtypetemplate.h"

#include "core/object.h"

#include "core/module.h"

EbmlElementTypeTemplate::EbmlElementTypeTemplate(const ObjectType &largeIntegerType)
    : ObjectTypeTemplate("EBMLElement", {"id"}),
      _largeIntegerType(largeIntegerType)
{
    setVirtual(true);
}

Parser *EbmlElementTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);

    Object* p_id = context.object().addVariable(_largeIntegerType, "id");
    Object* p_size = context.object().addVariable(_largeIntegerType, "size");

    context.object().setSize(p_id->size() + p_size->size() + 8 * p_size->value().toInteger());

    context.object().type().setParameter(0, p_id->value());

    return nullptr;
}
