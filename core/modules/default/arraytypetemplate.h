#ifndef ARRAYTYPETEMPLATE_H
#define ARRAYTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class ArrayTypeTemplate : public ObjectTypeTemplate
{
public:
    ArrayTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;

    virtual Variant attributeValue(const ObjectType& type, Attribute attribute) const override;
};

#endif // ARRAYTYPETEMPLATE_H
