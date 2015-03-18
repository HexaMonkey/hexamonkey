#ifndef MAPSCOPE_H
#define MAPSCOPE_H

#include <unordered_map>

#include "core/variable/variable.h"

class MapScope : public VariableImplementation
{
public:
    Variable field(const std::string& key) const;
    Variable field(const std::string& key, bool modifiable);
    void setField(const std::string& key, const Variable& variable);

protected:
    Variable doGetField(const Variant &key, bool modifiable);
    void doSetField(const Variant &key, const Variable &variable);

private:
    std::unordered_map<std::string, Variable> _fields;
};

#endif // MAPSCOPE_H
