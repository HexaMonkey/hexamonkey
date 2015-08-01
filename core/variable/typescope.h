#ifndef TYPESCOPEIMPLEMENTATION_H
#define TYPESCOPEIMPLEMENTATION_H

#include "core/variable/variable.h"

class Parser;
class ObjectType;

/**
 * @brief Scope implementation that gives access to a \link ObjectType type\endlink's parameters
 *
 * The arguments can either be access by their index or name.
 */
class AbstractTypeScope : public VariableImplementation
{
protected:
    virtual Variant doGetValue() override;
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;

    virtual ObjectType* modifiableType() = 0;
    virtual const ObjectType& constType() = 0;
};

/**
 * @brief Type scope implementation given a reference to a \link ObjectType type\endlink
 */
class TypeScope : public AbstractTypeScope
{
public:
    TypeScope(ObjectType& type, bool modifiable);
    TypeScope(const ObjectType& type);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    ObjectType* _type;
    const ObjectType& _constType;
};

/**
 * @brief Type scope implementation given a reference to a \link Parser parser\endlink
 */
class ParserTypeScope : public AbstractTypeScope
{
public:
    ParserTypeScope(std::shared_ptr<Parser> parser);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    std::shared_ptr<Parser> _parser;
};


#endif // TYPESCOPEIMPLEMENTATION_H
