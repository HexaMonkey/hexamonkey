#ifndef ARRAYSCOPE_H
#define ARRAYSCOPE_H

#include <vector>

#include "core/variable/variable.h"

class ArrayScope : public VariableImplementation
{
public:
    ArrayScope(VariableCollector& collector);

    void addField(const Variable& variable);
    Variable field(size_t index) const;
    size_t fieldCount() const;

protected:
    virtual Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    virtual void doSetField(const Variant &key, const Variable &variable) override;
    virtual void collect(const std::function<void(VariableMemory&)>& addAccessible) override;
    virtual bool isByRefOnly() override;

private:
    std::vector<VariableMemory> _fields;
};

#endif // ARRAYSCOPE_H
