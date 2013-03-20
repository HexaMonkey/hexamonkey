#ifndef DATASCOPE_H
#define DATASCOPE_H

#include "scope.h"
#include "typescope.h"

class Object;

class MutableObjectScope : public MutableScope
{
public:
    MutableObjectScope(Object& data);
protected:
    Variant* doGet(const Variant &key) const override;
    Scope* doGetScope(const Variant &key) const override;
private:
    Object& _object;
    MutableTypeScope _typeScope;
};

class ConstObjectScope : public ConstScope
{
public:
    ConstObjectScope(Object& data);
protected:
    const Variant* doCget(const Variant &key) const override;
    Scope* doGetScope(const Variant &key) const override;
private:
    Object& _object;
    ConstTypeScope _typeScope;
};

#endif // DATASCOPE_H
