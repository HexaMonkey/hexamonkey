#ifndef EBMLLARGEINTEGERTYPETEMPLATE_H
#define EBMLLARGEINTEGERTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class EbmlLargeIntegerTypeTemplate : public ObjectTypeTemplate
{
public:
    EbmlLargeIntegerTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;
};

#endif // EBMLLARGEINTEGERTYPETEMPLATE_H
