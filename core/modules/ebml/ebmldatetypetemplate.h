#ifndef EBMLDATETYPETEMPLATE_H
#define EBMLDATETYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class EbmlDateTypeTemplate : public ObjectTypeTemplate
{
public:
    EbmlDateTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

#endif // EBMLDATETYPETEMPLATE_H
