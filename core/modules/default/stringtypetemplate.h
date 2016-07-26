#ifndef STRINGTYPETEMPLATE_H
#define STRINGTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class StringTypeTemplate : public ObjectTypeTemplate
{
public:
    StringTypeTemplate();

    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;

};

class WStringTypeTemplate : public ObjectTypeTemplate
{
public:
    WStringTypeTemplate();

    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;

};

#endif // STRINGTYPETEMPLATE_H
