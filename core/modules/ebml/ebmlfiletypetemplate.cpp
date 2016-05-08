#include "ebmlfiletypetemplate.h"

#include "core/modules/ebml/ebmlmasterparser.h"

EbmlFileTypeTemplate::EbmlFileTypeTemplate(const ObjectTypeTemplate& fileTemplate)
    : ObjectTypeTemplate("EBMLFile"),
      _parent(new ObjectType(fileTemplate))
{
}

Parser *EbmlFileTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    return new EbmlMasterParser(option, module);
}

std::shared_ptr<ObjectType> EbmlFileTypeTemplate::parent(const ObjectType &) const
{
    return _parent;
}
