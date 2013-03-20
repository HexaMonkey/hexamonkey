#ifndef TYPESCOPE_H
#define TYPESCOPE_H

#include "scope.h"

class ObjectType;

class MutableTypeScope : public MutableScope
{
public:
    MutableTypeScope(ObjectType& type);
protected:
    Variant* doGet(const Variant &key) const override;
private:
    ObjectType& _type;
};

class ConstTypeScope : public ConstScope
{
public:
    ConstTypeScope(const ObjectType& type);
protected:
    const Variant* doCget(const Variant &key) const override;
private:
    const ObjectType& _type;
};



#endif // TYPESCOPE_H
