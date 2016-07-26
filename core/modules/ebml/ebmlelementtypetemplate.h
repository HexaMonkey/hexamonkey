#ifndef EBMLELEMENTTYPEELEMENT_H
#define EBMLELEMENTTYPEELEMENT_H

#include "core/objecttype.h"

class EbmlElementTypeTemplate : public ObjectTypeTemplate
{
public:
    EbmlElementTypeTemplate(const ObjectType& largeIntegerType);

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&) const override;

    ObjectType _largeIntegerType;
};

#endif // EBMLELEMENTTYPEELEMENT_H
