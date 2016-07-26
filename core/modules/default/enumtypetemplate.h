#ifndef ENUMTYPETEMPLATE_H
#define ENUMTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class EnumTypeTemplate : public ObjectTypeTemplate
{
public:
    EnumTypeTemplate();

    virtual Parser* parseOrGetParser(const ObjectType& type, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& type) const override;

    virtual Variant attributeValue(const ObjectType& type, Attribute attribute) const override;
};

#endif // ENUMTYPETEMPLATE_H
