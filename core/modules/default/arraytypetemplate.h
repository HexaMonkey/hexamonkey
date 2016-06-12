#ifndef ARRAYTYPETEMPLATE_H
#define ARRAYTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class ArrayTypeTemplate : public ObjectTypeTemplate
{
public:
    ArrayTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

#endif // ARRAYTYPETEMPLATE_H
