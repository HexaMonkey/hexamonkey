#include "specializer.h"

Specializer::Specializer()
    : _writable(true)
{

}

Specializer::Specializer(const Specializer &other)
    : _directSpecialization(other._directSpecialization),
      _nextSpecializations(other._nextSpecializations),
      _writable(false)
{
}

void Specializer::forward(const ObjectType &source, const ObjectType &destination)
{
    Specializer* currentSpecializer = this;
    int currentPassesCount = 0;

    for (int i = 0,  n = source.numberOfParameters(); i < n; ++i)
    {
        const Variant& value = source.parameterValue(i);
        if (value.isValueless()) {
            ++currentPassesCount;
        } else {
            for (;currentPassesCount > 0; -- currentPassesCount) {
                currentSpecializer = &(currentSpecializer->specializer());
            }

            currentSpecializer = &(currentSpecializer->specializer(value));
        }
    }

    currentSpecializer->_directSpecialization = destination;
}

const ObjectType &Specializer::specialize(const ObjectType &source) const
{
    const Specializer* currentSpecializer = this;
    const ObjectType* currentSpecialization = &(this->_directSpecialization);

    for (int i = 0,  n = source.numberOfParameters(); i < n; ++i)
    {
        const Variant& value = source.parameterValue(i);
        if (value.isValueless()) {
            currentSpecializer = specializerIfExists();
            if (!currentSpecializer) {
                break;
            }
        } else {
            currentSpecializer = currentSpecializer->specializerIfExists(value);
            if (currentSpecializer) {
                if (!currentSpecializer->_directSpecialization.isNull()) {
                    currentSpecialization = &(currentSpecializer->_directSpecialization);
                }
            } else {
                currentSpecializer = specializerIfExists();
                if (!currentSpecializer) {
                    break;
                }
            }
        }
    }

    return *currentSpecialization;
}

Specializer &Specializer::specializer(const Variant &value)
{
    if (!_nextSpecializations) {
        _nextSpecializations.reset(new std::unordered_map<Variant, Specializer>());
    }

    std::unordered_map<Variant, Specializer>& nextSpecializations = *_nextSpecializations;

    auto it = nextSpecializations.find(value);
    if (it != nextSpecializations.end()) {
        return it->second;
    } else {
        return nextSpecializations.emplace(value, Specializer()).first->second;
    }
}

Specializer &Specializer::specializer()
{
    if (!_nextSpecializations) {
        _nextSpecializations.reset(new std::unordered_map<Variant, Specializer>());
    }

    std::unordered_map<Variant, Specializer>& nextSpecializations = *_nextSpecializations;

    auto it = nextSpecializations.find(Variant::null());
    if (it != nextSpecializations.end()) {
        return it->second;
    } else {
        return nextSpecializations.emplace(Variant::null(), Specializer()).first->second;
    }
}

const Specializer *Specializer::specializerIfExists(const Variant &value) const
{
    if(!_nextSpecializations) {
        return nullptr;
    }

    std::unordered_map<Variant, Specializer>& nextSpecializations = *_nextSpecializations;

    auto it = nextSpecializations.find(value);
    if (it != nextSpecializations.end()) {
        return &(it->second);
    } else {
        return nullptr;
    }
}

const Specializer *Specializer::specializerIfExists() const
{
    if(!_nextSpecializations) {
        return nullptr;
    }

    std::unordered_map<Variant, Specializer>& nextSpecializations = *_nextSpecializations;

    auto it = nextSpecializations.find(Variant::null());
    if (it != nextSpecializations.end()) {
        return &(it->second);
    } else {
        return nullptr;
    }
}




