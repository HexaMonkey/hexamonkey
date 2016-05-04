#ifndef TUPLETYPETEMPLATE_H
#define TUPLETYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class TupleTypeTemplate : public ObjectTypeTemplate
{
public:
    TupleTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType, const Module&) const override;
};

#endif // TUPLETYPETEMPLATE_H
