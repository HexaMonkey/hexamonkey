#include "ebmlextensiontypetemplate.h"


#include "core/modules/ebml/ebmlmasterparser.h"

#include "core/variable/objectattributes.h"

EbmlMasterTypeTemplate::EbmlMasterTypeTemplate(std::shared_ptr<ObjectType> elementType)
    : FixedParentTypeTemplate("MasterElement", elementType)
{
}

Parser *EbmlMasterTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    return new EbmlMasterParser(option, module);
}


EbmlIntegerTypeTemplate::EbmlIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& intTypeTemplate)
    : FixedParentTypeTemplate("IntegerElement", elementType),
      _intTypeTemplate(intTypeTemplate)
{
}

Parser *EbmlIntegerTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    ObjectType intType(_intTypeTemplate, context.object().availableSize());
    Object* child = context.object().addVariable(intType,"payload", module);
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}

EbmlUIntegerTypeTemplate::EbmlUIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& uintTypeTemplate)
    : FixedParentTypeTemplate("UIntegerElement", elementType),
      _uintTypeTemplate(uintTypeTemplate)
{
}

Parser *EbmlUIntegerTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    ObjectType intType(_uintTypeTemplate, context.object().availableSize());
    Object* child = context.object().addVariable(intType,"payload", module);
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlFloatTypeTemplate::EbmlFloatTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate &floatTypeTemplate, const ObjectTypeTemplate &doubleTypeTemplate)
    : FixedParentTypeTemplate("FloatElement", elementType),
      _floatType(floatTypeTemplate),
      _doubleType(doubleTypeTemplate)
{
}

Parser *EbmlFloatTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    Object* child = context.object().addVariable(context.object().availableSize() == 64 ? _doubleType : _floatType,"payload", module);
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlStringTypeTemplate::EbmlStringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate &stringTypeTemplate)
    : FixedParentTypeTemplate("StringElement", elementType),
      _stringTypeTemplate(stringTypeTemplate)

{
}

Parser *EbmlStringTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    ObjectType type(_stringTypeTemplate, context.object().availableSize()/8);
    Object* child = context.object().addVariable(type, "payload", module);
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlUtf8StringTypeTemplate::EbmlUtf8StringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate &stringTypeTemplate)
    : FixedParentTypeTemplate("UTF8StringElement", elementType),
      _stringTypeTemplate(stringTypeTemplate)
{
}

Parser *EbmlUtf8StringTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    ObjectType type(_stringTypeTemplate, context.object().availableSize()/8);
    Object* child = context.object().addVariable(type, "payload", module);
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlDateElementTypeTemplate::EbmlDateElementTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate &dateTypeTemplate)
    : FixedParentTypeTemplate("DateElement", elementType),
      _dateType(dateTypeTemplate)
{
}

Parser *EbmlDateElementTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    Object* child = context.object().addVariable(_dateType, "payload", module);
    if (child) {
        context.object().setValue(child->value());
        context.object().attributes()->addNumbered().setValue(child->attributes()->getNumbered(0));
    }
    return nullptr;
}


EbmlBinaryTypeTemplate::EbmlBinaryTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate &dataTypeTemplate)
    : FixedParentTypeTemplate("BinaryElement", elementType),
      _dataTypeTemplate(dataTypeTemplate)
{
}

Parser *EbmlBinaryTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option, const Module &module) const
{
    Object::ParsingContext context(option);
    ObjectType type(_dataTypeTemplate, context.object().availableSize());
    Object* child = context.object().addVariable(type, "payload", module);
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}
