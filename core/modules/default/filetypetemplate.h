#ifndef FILETYPETEMPLATE_H
#define FILETYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class FileTypeTemplate : public ObjectTypeTemplate
{
public:
    FileTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;
};

#endif // FILETYPETEMPLATE_H
