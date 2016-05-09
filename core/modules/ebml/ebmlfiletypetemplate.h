#ifndef EBMLFILETYPETEMPLATE_H
#define EBMLFILETYPETEMPLATE_H

#include "core/fixedparenttypetemplate.h"

class EbmlFileTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlFileTypeTemplate(const ObjectTypeTemplate& fileTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&, const Module &module) const override;
};

#endif // EBMLFILETYPETEMPLATE_H
