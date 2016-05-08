#ifndef EBMLFILETYPETEMPLATE_H
#define EBMLFILETYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class EbmlFileTypeTemplate : public ObjectTypeTemplate
{
public:
    EbmlFileTypeTemplate(const ObjectTypeTemplate& fileTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType&, ParsingOption&, const Module &module) const override;

    virtual std::shared_ptr<ObjectType> parent(const ObjectType&) const override;

    std::shared_ptr<ObjectType> _parent;
};

#endif // EBMLFILETYPETEMPLATE_H
