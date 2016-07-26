#ifndef TUPLETYPETEMPLATE_H
#define TUPLETYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class TupleTypeTemplate : public ObjectTypeTemplate
{
public:
    TupleTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;

    virtual Variant attributeValue(const ObjectType& type, Attribute attribute) const override;
};

#endif // TUPLETYPETEMPLATE_H
