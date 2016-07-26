#include "ebmlextensiontypetemplate.h"


#include "core/modules/ebml/ebmlmasterparser.h"

#include "core/variable/objectattributes.h"

#include "core/module.h"

EbmlMasterTypeTemplate::EbmlMasterTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& elementType2)
    : FixedParentTypeTemplate("MasterElement", elementType),
      _elementType(elementType2)
{
}

Parser *EbmlMasterTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    return new EbmlMasterParser(option, _elementType);
}


EbmlIntegerTypeTemplate::EbmlIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& intType)
    : FixedParentTypeTemplate("IntegerElement", elementType),
      _intType(intType)
{
}

Parser *EbmlIntegerTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    _intType.setParameter(0, context.object().availableSize());
    Object* child = context.object().addVariable(_intType,"payload");
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}

EbmlUIntegerTypeTemplate::EbmlUIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& uintType)
    : FixedParentTypeTemplate("UIntegerElement", elementType),
      _uintType(uintType)
{
}

Parser *EbmlUIntegerTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    _uintType.setParameter(0, context.object().availableSize());
    Object* child = context.object().addVariable(_uintType,"payload");
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlFloatTypeTemplate::EbmlFloatTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &floatType, const ObjectType &doubleType)
    : FixedParentTypeTemplate("FloatElement", elementType),
      _floatType(floatType),
      _doubleType(doubleType)
{
}

Parser *EbmlFloatTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    Object* child = context.object().addVariable(context.object().availableSize() == 64 ? _doubleType : _floatType, "payload");
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlStringTypeTemplate::EbmlStringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &stringType)
    : FixedParentTypeTemplate("StringElement", elementType),
      _stringType(stringType)

{
}

Parser *EbmlStringTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    _stringType.setParameter(0, context.object().availableSize()/8);
    Object* child = context.object().addVariable(_stringType, "payload");
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlUtf8StringTypeTemplate::EbmlUtf8StringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &stringType)
    : FixedParentTypeTemplate("UTF8StringElement", elementType),
      _stringType(stringType)
{
}

Parser *EbmlUtf8StringTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    _stringType.setParameter(0, context.object().availableSize()/8);
    Object* child = context.object().addVariable(_stringType, "payload");
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}


EbmlDateElementTypeTemplate::EbmlDateElementTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &dateTypeTemplate)
    : FixedParentTypeTemplate("DateElement", elementType),
      _dateType(dateTypeTemplate)
{
}

Parser *EbmlDateElementTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    Object* child = context.object().addVariable(_dateType, "payload");
    if (child) {
        context.object().setValue(child->value());
        context.object().attributes()->addNumbered().setValue(child->attributes()->getNumbered(0));
    }
    return nullptr;
}


EbmlBinaryTypeTemplate::EbmlBinaryTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &dataType)
    : FixedParentTypeTemplate("BinaryElement", elementType),
      _dataType(dataType)
{
}

Parser *EbmlBinaryTypeTemplate::parseOrGetParser(const ObjectType &, ParsingOption &option) const
{
    Object::ParsingContext context(option);
    _dataType.setParameter(0, context.object().availableSize());
    Object* child = context.object().addVariable(_dataType, "payload");
    if (child) {
        context.object().setValue(child->value());
    }
    return nullptr;
}
