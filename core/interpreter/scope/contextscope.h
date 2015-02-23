#ifndef CONTEXTSCOPE_H
#define CONTEXTSCOPE_H


#include "core/interpreter/scope/scope.h"

class Object;

class ContextScope : public Scope
{
public:
    ContextScope(Object& object);

protected:
    virtual Variable doGet(const Variant &key, bool modifiable) override;

private:
    Object& _object;
};
#endif // CONTEXTSCOPE_H
