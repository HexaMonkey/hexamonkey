#ifndef ARRAYSCOPE_H
#define ARRAYSCOPE_H

#include <vector>

#include "core/variable/variable.h"

class ArrayScope : public VariableImplementation
{
public:
    void addField(const Variable& variable);
    Variable& field(size_t index);
    const Variable &field(size_t index) const;
    size_t fieldCount() const;

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded);
    virtual void doSetField(const Variant &key, const Variable &variable);

private:
    std::vector<Variable> _fields;
};

#endif // ARRAYSCOPE_H
