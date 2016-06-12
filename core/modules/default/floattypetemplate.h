#ifndef FLOATTYPETEMPLATE_H
#define FLOATTYPETEMPLATE_H

#include "core/objecttypetemplate.h"

class FloatTypeTemplate : public ObjectTypeTemplate
{
public:
    FloatTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

class DoubleTypeTemplate : public ObjectTypeTemplate
{
public:
    DoubleTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

class FixedFloatTypeTemplate : public ObjectTypeTemplate
{
public:
    FixedFloatTypeTemplate();

private:
    virtual Parser* parseOrGetParser(const ObjectType& objectType, ParsingOption& option, const Module&) const override;

    virtual int64_t fixedSize(const ObjectType& objectType) const override;
};

#endif // FLOATTYPETEMPLATE_H
