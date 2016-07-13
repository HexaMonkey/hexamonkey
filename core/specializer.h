#ifndef SPECIALIZER_H
#define SPECIALIZER_H

#include <unordered_map>

#include <memory>
#include "core/varianthash.h"
#include "core/objecttype.h"

class Specializer
{
public:
    Specializer();
    Specializer(const ObjectType& source, const ObjectType& destination);

    Specializer(const Specializer& other);

public:
    void forward(const ObjectType& source, const ObjectType& destination);
    const ObjectType& specialize(const ObjectType& source) const;


    std::ostream& display(std::ostream& out) const;
private:
    Specializer& specializer(const Variant& value);
    Specializer& specializer();


    const Specializer* specializerIfExists(const Variant &value) const;
    const Specializer* specializerIfExists() const;

    inline void detach() {
        if (!_writable) {
            if (_nextSpecializations) {
                _nextSpecializations.reset(new std::unordered_map<Variant, Specializer>(*_nextSpecializations));
            }
            _writable = true;
        }
    }

    ObjectType _directSpecialization;
    std::shared_ptr<std::unordered_map<Variant, Specializer> > _nextSpecializations;
    bool _writable;
};

std::ostream& operator<<(std::ostream& out, const Specializer& specializer);

#endif // SPECIALIZER_H
