#include "ebmlfiletypetemplate.h"

#include "core/modules/ebml/ebmlmasterparser.h"

EbmlFileTypeTemplate::EbmlFileTypeTemplate(const ObjectType& fileType, const ObjectType& elementType)
    : FixedParentTypeTemplate("EBMLFile", std::make_shared<ObjectType>(fileType)),
      _elementType(elementType)
{
}

Parser *EbmlFileTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &) const
{
    return new EbmlMasterParser(option, _elementType);
}
