#ifndef INTEGERTYPETEMPLATE_H
#define INTEGERTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class IntegerTypeTemplate : public ObjectTypeTemplate
{
public:
    IntegerTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

class UIntegerTypeTemplate : public ObjectTypeTemplate
{
public:
    UIntegerTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

class ByteTypeTemplate : public ObjectTypeTemplate
{
public:
    ByteTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

class BitsetTypeTemplate : public ObjectTypeTemplate
{
public:
    BitsetTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

class UuidTypeTemplate : public ObjectTypeTemplate
{
public:
    UuidTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};



#endif // INTEGERTYPETEMPLATE_H
