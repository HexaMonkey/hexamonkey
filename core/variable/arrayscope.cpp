#include "arrayscope.h"

#include "core/log/logmanager.h"
#include "core/variable/variablecollector.h"

#include <unordered_map>

#define A_COUNT 0

const std::unordered_map<std::string, int> reserved = {
    {"@count", A_COUNT}
};

ArrayScope::ArrayScope(VariableCollector &collector) : VariableImplementation(collector)
{
}

void ArrayScope::addField(const Variable &variable)
{
    _fields.emplace_back(variable);
}

Variable ArrayScope::field(size_t index) const
{
    return _fields[index];
}

size_t ArrayScope::fieldCount() const
{
    return _fields.size();
}

Variable ArrayScope::doGetField(const Variant &key, bool /*modifiable*/, bool createIfNeeded)
{
    if (key.isValueless()) {
        if (createIfNeeded) {
            addField(collector().null());
            return _fields.back();
        } else if (_fields.size() > 0) {
            return _fields.back();
        } else {
            return Variable();
        }
    } else if (key.hasNumericalType()) {
        const int64_t index = key.toInteger();
        if (index >= 0) {
            if (createIfNeeded) {
                while (index >= _fields.size()) {
                    addField(collector().null());
                }
                return _fields[index];
            } else if (index < _fields.size()) {
                return _fields[index];
            } else {
                return Variable();
            }
        } else {
            if (createIfNeeded) {
                Log::error("Trying to get an array item with a negative index ", index);
            }
            return Variable();
        }
    } else if (key.type() == Variant::stringType) {

        const std::string& name = key.toString();
        if (name[0] == '@') {
            auto it = reserved.find(name);
            if (it == reserved.end()) {
                Log::error("Unknown reserved field", key);
                return Variable();
            } else {
                switch (it->second) {
                    case A_COUNT:
                        return collector().copy(_fields.size(), false);

                    default:
                        return Variable();
                }
            }
        } else {
            if (createIfNeeded) {
                Log::error("Invalid index ", key, " for array");
            }
            return Variable();
        }
    } else {
        if (createIfNeeded) {
            Log::error("Invalid index ", key, " for array");
        }
        return Variable();
    }
}

void ArrayScope::doSetField(const Variant &key, const Variable &variable)
{
    if (key.isValueless()) {
        addField(variable);
    } else if (key.hasNumericalType()) {
        const int64_t index = key.toInteger();
        if (index >= 0) {

            while (index > _fields.size()) {
                addField(collector().null());
            }

            if (index >= _fields.size()) {
                addField(variable);
            } else {
                _fields[index] = variable;
            }

        } else {
            Log::error("Trying to set an array item with a negative index");
        }
    } else {
        Log::error("Invalid index type for array");
    }
}

void ArrayScope::collect(const std::function<void(VariableMemory&)>& addAccessible)
{
    for (auto& variableMemory : _fields) {
        addAccessible(variableMemory);
    }
}

bool ArrayScope::isByRefOnly()
{
    return true;
}
