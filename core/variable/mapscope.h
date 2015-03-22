#ifndef MAPSCOPE_H
#define MAPSCOPE_H

#include <unordered_map>

#include "core/variable/variable.h"
#include "core/varianthash.h"

class MapScope : public VariableImplementation
{
public:
    Variable field(const Variant& key) const;
    Variable field(const Variant& key, bool createIfNeeded);
    void setField(const Variant& key, const Variable& variable);

protected:
    Variable doGetField(const Variant &key, bool modifiable, bool createIfNeeded);
    void doSetField(const Variant &key, const Variable &variable);

private:
    std::unordered_map<Variant, Variable> _fields;
};

#endif // MAPSCOPE_H
