#ifndef EBMLEXTENSIONTYPETEMPLATE_H
#define EBMLEXTENSIONTYPETEMPLATE_H

#include "core/fixedparenttypetemplate.h"

#include "core/objecttype.h"

class EbmlMasterTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlMasterTypeTemplate(std::shared_ptr<ObjectType> elementType);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;
};

class EbmlIntegerTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& intTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    const ObjectTypeTemplate& _intTypeTemplate;
};

class EbmlUIntegerTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlUIntegerTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& uintTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    const ObjectTypeTemplate& _uintTypeTemplate;
};

class EbmlFloatTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlFloatTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& floatTypeTemplate, const ObjectTypeTemplate& doubleTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    ObjectType _floatType;
    ObjectType _doubleType;
};

class EbmlStringTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlStringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& stringTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    const ObjectTypeTemplate& _stringTypeTemplate;
};

class EbmlUtf8StringTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlUtf8StringTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& stringTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    const ObjectTypeTemplate& _stringTypeTemplate;
};

class EbmlDateElementTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlDateElementTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& dateTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    ObjectType _dateType;
};

class EbmlBinaryTypeTemplate : public FixedParentTypeTemplate
{
public:
    EbmlBinaryTypeTemplate(std::shared_ptr<ObjectType> elementType, const ObjectTypeTemplate& dataTypeTemplate);

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    const ObjectTypeTemplate& _dataTypeTemplate;
};

#endif // EBMLEXTENSIONTYPETEMPLATE_H
