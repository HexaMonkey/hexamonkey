#ifndef CONTEXTSCOPE_H
#define CONTEXTSCOPE_H


#include "core/interpreter/scope/scope.h"

class Object;

/**
 * @brief Scope accessing the context of an \link Object object\endlink
 */
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
