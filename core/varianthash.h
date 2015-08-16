#ifndef VARIANTHASH_H
#define VARIANTHASH_H

#include "core/variant.h"
#include "core/objecttype.h"
#include "core/objecttypetemplate.h"

#include <functional>

/** @cond */
namespace std
{
    template<>
    struct hash<Variant>
    {
        std::size_t operator()(Variant const& value) const
        {
            std::size_t result;
            switch (value._type & Variant::typeMask) {
                case Variant::integerType:
                    if (value._data.l >= 0) {
                        result = std::hash<unsigned long long>()(value._data.ul);
                    } else {
                        result = std::hash<long long>()(value._data.l);
                    }
                    break;

                case Variant::unsignedIntegerType:
                    result = std::hash<unsigned long long>()(value._data.ul);
                    break;

                case Variant::floatingType:
                {
                    if (value._data.f >= 0) {
                        result = std::hash<long long>()((long long) value._data.f);
                    } else {
                        result = std::hash<unsigned long long>()((unsigned long long) value._data.f);
                    }
                    break;
                }

                case Variant::stringType:
                {
                    result = std::hash<std::string>()(value._data.s->first);
                    break;
                }

                case Variant::objectType:
                {
                    result = std::hash<std::string>()(value._data.t->first.typeTemplate().name());
                    break;
                }

                case Variant::nullType:
                    result = 0x80f0f0f0;
                    break;

                case Variant::undefinedType:
                    result = 0x80d0d0d0;
                    break;

                default:
                    result = 0;
                    break;
            }
            return result;
        }
    };
}
/** @endcond */

#endif // VARIANTHASH_H
