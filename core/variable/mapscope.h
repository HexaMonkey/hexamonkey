#ifndef MAPSCOPE_H
#define MAPSCOPE_H

#include <unordered_map>

#include "core/variable/variable.h"
#include "core/varianthash.h"

class MapScope : public VariableImplementation
{
public:
    MapScope(VariableCollector& collector);

    Variable field(const Variant& key) const;
    Variable field(const Variant& key, bool createIfNeeded);
    void setField(const Variant& key, const Variable& variable);

protected:
    Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded) override;
    void doSetField(const Variant &key, const Variable &variable) override;
    void collect(const std::function<void(VariableMemory&)>& addAccessible) override;

private:
    std::unordered_map<Variant, VariableMemory> _fields;
};

#endif // MAPSCOPE_H
