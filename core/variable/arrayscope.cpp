#include "arrayscope.h"

#include "core/log/logmanager.h"

void ArrayScope::addField(const Variable &variable)
{
    _fields.push_back(variable);
}

Variable &ArrayScope::field(size_t index)
{
    return _fields[index];
}

const Variable &ArrayScope::field(size_t index) const
{
    return _fields[index];
}

size_t ArrayScope::fieldCount() const
{
    return _fields.size();
}

Variable ArrayScope::doGetField(const Variant &key, bool modifiable)
{
    if (key.isValueless()) {
        if (modifiable) {
            addField(Variable::null());
            return _fields.back();
        } else if (_fields.size() > 0) {
            return _fields.back();
        } else {
            return Variable();
        }
    } else if (key.hasNumericalType()) {
        const int64_t index = key.toInteger();
        if (index >= 0) {
            if (modifiable) {
                while (index >= _fields.size()) {
                    addField(Variable::null());
                }
                return _fields[index];
            } else if (index < _fields.size()) {
                return _fields[index];
            } else {
                return Variable();
            }
        } else {
            if (modifiable) {
                Log::error("Trying to get an array item with a negative index");
            }
            return Variable();
        }
    } else {
        if (modifiable) {
            Log::error("Invalid index type for array");
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
                addField(Variable::null());
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
