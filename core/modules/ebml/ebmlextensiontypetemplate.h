#ifndef EBMLEXTENSIONTYPETEMPLATE_H
#define EBMLEXTENSIONTYPETEMPLATE_H

#include "core/fixedparenttypetemplate.h"

#include "core/objecttype.h"

class EbmlMasterTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlMasterTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& elementType2);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    ObjectType _elementType;
};

class EbmlIntegerTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& intType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    mutable ObjectType _intType;
};

class EbmlUIntegerTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlUIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &uintType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

   mutable ObjectType _uintType;
};

class EbmlFloatTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlFloatTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& floatType, const ObjectType& doubleType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    ObjectType _floatType;
    ObjectType _doubleType;
};

class EbmlStringTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlStringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& stringType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    mutable ObjectType _stringType;
};

class EbmlUtf8StringTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlUtf8StringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &stringType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    mutable ObjectType _stringType;
};

class EbmlDateElementTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlDateElementTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType &dateTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    mutable ObjectType _dateType;
};

class EbmlBinaryTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlBinaryTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectType& dataType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    mutable ObjectType _dataType;
};

#endif // EBMLEXTENSIONTYPETEMPLATE_H
