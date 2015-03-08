#ifndef CONSTSCOPE_H
#define CONSTSCOPE_H

#include "core/interpreter/scope/scope.h"

/**
 * @brief Scope wrapper permitting only const actions
 */
class ConstScope : public Scope
{
public:
    ConstScope(Ptr scopePtr);

protected:
    Variable doGet(const Variant &key, bool modifiable) override;
    Ptr doGetScope(const Variant &key) override;
    Variable doDeclare(const Variant &key, const Variant &initialValue) override;
    Variable getValue(bool) override;

private:
    Ptr _scopePtr;
    Scope& _scope;
};

#endif // CONSTSCOPE_H
