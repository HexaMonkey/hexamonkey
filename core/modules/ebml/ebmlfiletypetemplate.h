#ifndef EBMLFILETYPETEMPLATE_H
#define EBMLFILETYPETEMPLATE_H

#include "core/fixedparenttypetemplate.h"

#include "core/objecttype.h"

class EbmlFileTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlFileTypeTemplate(const ObjectType& fileType, const ObjectType &elementType);

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&, const Module &module) const override;

    ObjectType _elementType;
};

#endif // EBMLFILETYPETEMPLATE_H
