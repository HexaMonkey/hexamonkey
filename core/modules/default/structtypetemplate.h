#ifndef STRUCTTYPETEMPLATE_H
#define STRUCTTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class StructTypeTemplate : public ObjectTypeTemplate
{
public:
    StructTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;

    virtual Variant attributeValue(const ObjectType& type, Attribute attribute) const override;
};

#endif // STRUCTTYPETEMPLATE_H
