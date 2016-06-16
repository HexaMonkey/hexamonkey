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




