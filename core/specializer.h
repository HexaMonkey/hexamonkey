#ifndef SPECIALIZER_H
#define SPECIALIZER_H

#include <unordered_map>

#include <memory>
#include "core/varianthash.h"
#include "core/objecttype.h"

class Specializer
{
    Specializer();

    Specializer(const Specializer& other);
private:
    inline void detach() {
        if (!_writable) {
            if (_nextSpecializations) {
                _nextSpecializations.reset(new std::unordered_map<Variant, Specializer> >(*_nextSpecializations));
            }
            _writable = true;
        }
    }

    ObjectType _directSpecialization;
    std::shared_ptr<std::unordered_map<Variant, Specializer> > _nextSpecializations;
    bool _writable;
};



#endif // SPECIALIZER_H
