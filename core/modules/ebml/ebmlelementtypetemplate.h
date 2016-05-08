#ifndef EBMLELEMENTTYPEELEMENT_H
#define EBMLELEMENTTYPEELEMENT_H

#include "core/objecttypetemplate.h"

class EbmlElementTypeTemplate : public ObjectTypeTemplate
{
public:
    EbmlElementTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&, const Module &module) const override;
};

#endif // EBMLELEMENTTYPEELEMENT_H
