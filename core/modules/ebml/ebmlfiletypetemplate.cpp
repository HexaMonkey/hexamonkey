#include "ebmlfiletypetemplate.h"

#include "core/modules/ebml/ebmlmasterparser.h"

EbmlFileTypeTemplate::EbmlFileTypeTemplate(const ObjectType& fileType)
    : FixedParentTypeTemplate("EBMLFile", std::make_shared<ObjectType>(fileType))
{
}

Parser *EbmlFileTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    return new EbmlMasterParser(option, module);
}
