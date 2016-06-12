#ifndef DATATYPETEMPLATE_H
#define DATATYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class DataTypeTemplate : public ObjectTypeTemplate
{
public:
    DataTypeTemplate();

    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

#endif // DATATYPETEMPLATE_H
