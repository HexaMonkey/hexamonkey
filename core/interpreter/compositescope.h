#ifndef COMPOSITESCOPE_H
#define COMPOSITESCOPE_H

#include <vector>

#include "scope.h"

class CompositeScope : public Scope
{
public:
    void addScope(Scope&scope);
    Variant* get(const Variant &key) const override;
    const Variant* cget(const Variant &key) const override;

protected:
    Scope* doGetScope(const Variant &key) const override;
    Variant* doDeclare(const Variant &key) const override;
private:
    std::vector<Scope*> _scopes;
};

#endif // COMPOSITESCOPE_H
