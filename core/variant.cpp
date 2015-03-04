//This file is part of the HexaMonkey project, a multimedia analyser
//Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either version 2
//of the License, or (at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.

//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

#include <algorithm> //swap
#include <sstream>
#include <iomanip>

#include "core/variant.h"
#include "core/objecttype.h"
#include "core/log/logmanager.h"

const std::vector<std::string>& typeNames = {"unknown", "integer", "unsigned integer" , "float", "string","object type"};

const std::string emptyString;
const ObjectType emptyType;

Variant::Variant() : _type(undefinedType)
{
}

Variant Variant::null()
{
    Variant variant;
    variant._type = nullType;
    return variant;
}

Variant::Variant(const Variant& other) : _type(other._type)
{
    switch(_type & superTypeMask)
    {
        case numericalType:
            _data = other._data;
            break;

        case stringType:
            _data.s = new std::string(*other._data.s);
            break;

        case objectType:
            _data.t = new ObjectType(*other._data.t);
            break;

        default:
            break;
    }
}

Variant::Variant(bool l) : _type(integerType)
{
    _data.l = l;
}

Variant::Variant(char l) : _type(integerType)
{
    _data.l = l;
}

Variant::Variant(int l) : _type(integerType)
{
    _data.l = l;
}

Variant::Variant(long l) : _type(integerType)
{
    _data.l = l;
}

Variant::Variant(long long l) : _type(integerType)
{
    _data.l = l;
}

Variant::Variant(unsigned char ul) : _type(unsignedIntegerType)
{
    _data.ul = ul;
}

Variant::Variant(unsigned int ul) : _type(unsignedIntegerType)
{
    _data.ul = ul;
}

Variant::Variant(unsigned long ul) : _type(unsignedIntegerType)
{
    _data.ul = ul;
}

Variant::Variant(unsigned long long ul) : _type(unsignedIntegerType)
{
    _data.ul = ul;
}

Variant::Variant(float f) : _type(floatingType)
{
    _data.f = f;
}

Variant::Variant(double f) : _type(floatingType)
{
    _data.f = f;
}

Variant::Variant(const std::string& s) : _type(stringType)
{
    _data.s = new std::string(s);
}

Variant::Variant(const char* s) : _type(stringType)
{
    _data.s = new std::string(s);
}

Variant::Variant(const ObjectType& t) : _type(objectType)
{
    _data.t = new ObjectType(t);
}

Variant::~Variant()
{
    clear();
}

void swap(Variant &a, Variant &b)
{
    using std::swap;

    swap(a._type, b._type);
    swap(a._data, b._data);
}

Variant& Variant::operator=(Variant other)
{
    swap(*this, other);
    return *this;
}

void Variant::setValue(Variant other)
{
    swap(*this, other);
}

void Variant::setValue(bool l)
{
    clear();
    _type = integerType;
    _data.l = l;
}

void Variant::setValue(char l)
{
    clear();
    _type = integerType;
    _data.l = l;
}

void Variant::setValue(int l)
{
    clear();
    _type = integerType;
    _data.l = l;
}

void Variant::setValue(long l)
{
    clear();
    _type = integerType;
    _data.l = l;
}

void Variant::setValue(long long l)
{
    clear();
    _type = integerType;
    _data.l = l;
}

void Variant::setValue(unsigned char ul)
{
    clear();
    _type = unsignedIntegerType;
    _data.ul = ul;
}

void Variant::setValue(unsigned int ul)
{
    clear();
    _type = unsignedIntegerType;
    _data.ul = ul;
}

void Variant::setValue(unsigned long ul)
{
    clear();
    _type = unsignedIntegerType;
    _data.ul = ul;
}

void Variant::setValue(unsigned long long ul)
{
    clear();
    _type = unsignedIntegerType;
    _data.ul = ul;
}

void Variant::setValue(float f)
{
    clear();
    _type = floatingType;
    _data.f = f;
}

void Variant::setValue(double f)
{
    clear();
    _type = floatingType;
    _data.f = f;
}
void Variant::setValue(const std::string& s)
{
    clear();
    _type = stringType;
    _data.s = new std::string(s);
}

void Variant::setValue(const char* s)
{
    clear();
    _type = stringType;
    _data.s = new std::string(s);
}

void Variant::setValue(const ObjectType& t)
{
    clear();
    _type = objectType;
    _data.t = new ObjectType(t);
}

void Variant::clear()
{
    switch(_type & superTypeMask)
    {
        case stringType:
            delete _data.s;
            break;
        case objectType:
            delete _data.t;
            break;
        default:
            break;
    }
    _type = valuelessType;
}


bool Variant::canConvertTo(Variant::Type otherType) const
{
    return (_type & superTypeMask) == (otherType & superTypeMask);
}

Variant& Variant::convertTo(Variant::Type newType)
{
    uint8_t type = _type & typeMask;

    if (type != newType) {
        switch(type)
        {
            case integerType:
                switch(newType)
                {
                    case unsignedIntegerType:
                        _data.ul = (unsigned long long) _data.l;
                        break;

                    case floatingType:
                        _data.f = (double) _data.l;
                        break;

                    default:
                        Log::error("Invalid type conversion : ", typeNames[type], " to ", typeNames[newType]);
                        clear();
                }
                break;

            case unsignedIntegerType:
                switch(newType)
                {
                    case integerType:
                        _data.l = (long long) _data.ul;
                        break;

                    case floatingType:
                        _data.f = (double) _data.ul;
                        break;

                    default:
                        Log::error("Invalid type conversion : ", typeNames[type], " to ", typeNames[newType]);
                        clear();
                }
                break;

            case floatingType:
                switch(newType)
                {
                    case integerType:
                        _data.l = (long long) _data.f;
                        break;

                    case unsignedIntegerType:
                        _data.ul = (double) _data.f;
                        break;

                    default:
                        Log::error("Invalid type conversion : ", typeNames[type], " to ", typeNames[newType]);
                        clear();
                }
                break;

            default:
                Log::error("Invalid type conversion : ", typeNames[type], " to ",typeNames[newType]);
                clear();
        }
    }
    _type = (_type & ~typeMask)|newType;
    return *this;
}

long long Variant::toInteger() const
{
    if ((_type & superTypeMask) == numericalType) {
        if ((_type & typeMask) == floatingType) {
            return _data.f;
        } else {
            return _data.l;
        }
    } else {
        return 0LL;
    }
}

unsigned long long Variant::toUnsignedInteger() const
{
    if ((_type & superTypeMask) == numericalType) {
        if ((_type & typeMask) == floatingType) {
            return _data.f;
        } else {
            return _data.ul;
        }
    } else {
        return 0LL;
    }
}

double Variant::toDouble() const
{
    if ((_type & superTypeMask) == numericalType) {
        if ((_type & typeMask) == floatingType) {
            return _data.f;
        } else {
            return _data.l;
        }
    } else {
        return 0.;
    }
}

const std::string& Variant::toString() const
{
    if((_type & typeMask) == stringType) {
        return *_data.s;
    } else {
        return emptyString;
    }
}

const ObjectType& Variant::toObjectType() const
{
    if((_type & typeMask) == objectType) {
        return *_data.t;
    } else {
        return emptyType;
    }
}

ObjectType& Variant::toObjectType()
{
    return *_data.t;
}

bool Variant::toBool() const
{
    switch(_type & typeMask)
    {
        case integerType:
            return _data.l != 0LL;
        case unsignedIntegerType:
            return _data.ul != 0LL;
        case floatingType:
            return _data.f != 0.;
        case stringType:
            return !_data.s->empty();
        case objectType:
            return !_data.t->isNull();
        default:
            return false;

    }
}

Variant::Type Variant::type() const
{
    switch (_type & typeMask)
    {
        case integerType:
            return integerType;

        case unsignedIntegerType:
            return unsignedIntegerType;

        case floatingType:
            return floatingType;

        case stringType:
            return stringType;

        case objectType:
            return objectType;

        case nullType:
            return nullType;

        default:
            return undefinedType;
    }
    return valuelessType;
}

bool Variant::hasNumericalType() const
{
    return (_type & superTypeMask) == numericalType;
}

bool Variant::isValueless() const
{
    return (_type & superTypeMask) == valuelessType;
}

bool Variant::isNull() const
{
    return (_type & typeMask) == nullType;
}

bool Variant::isUndefined() const
{
    return (_type & typeMask) == undefinedType;
}

void Variant::setDisplayType(Variant::Display display)
{
    _type = (_type & ~displayMask) | display;
}

std::ostream& Variant::display(std::ostream& out) const
{
    uint8_t displayType = (_type & displayMask);
    uint8_t type = (_type & typeMask);
    if (displayType == binary && (_type & typeMask & unsignedIntegerType) == unsignedIntegerType) {
        unsigned long long ul;
        if (type == integerType && _data.l < 0) {
            ul = (unsigned long long) (-_data.l);
            out << "-";
        } else {
            ul = _data.ul;
        }
        out << "0b";
        bool displayedOne = false;
        for (int i = 0; i < 64; ++ i) {
            if (ul & 0x8000000000000000) {
                out << "1";
            } else if (displayedOne) {
                out << "0";
            }
            ul <<= 1;
        }

        if (!displayedOne) {
            out << "0";
        }

    } else {
        switch (displayType) {
            case octal:
                out << std::oct << std::showbase;
                break;

            case hexadecimal:
                out << std::hex << std::showbase;
                break;
        }


        switch (type)
        {
            case integerType:
                out<<_data.l;
                break;

            case unsignedIntegerType:
                out<<_data.ul;
                break;

            case floatingType:
                out<<_data.f;
                break;

            case stringType:
                out<<*_data.s;
                break;

            case objectType:
                out<<*_data.t;
                break;

            default:
                out<<"NULL";
            break;
        }

        out << std::dec;
    }
    return out;
}

bool operator==(const Variant& a, const Variant& b)
{
    uint8_t aSuperType = a._type & Variant::superTypeMask;
    uint8_t bSuperType = b._type & Variant::superTypeMask;

    if (aSuperType == bSuperType) {
        switch (aSuperType) {
            case Variant::numericalType:
                {
                    bool aFloating = (a._type & Variant::typeMask) == Variant::floatingType;
                    bool bFloating = (b._type & Variant::typeMask) == Variant::floatingType;
                    if (aFloating && bFloating) {
                        return a._data.f == b._data.f;
                    } else if (aFloating) {
                        return a._data.f == b._data.l;
                    } else if (bFloating) {
                        return a._data.l == b._data.f;
                    } else {
                        return a._data.l == b._data.l;
                    }
                }

            case Variant::stringType:
                return *a._data.s == *b._data.s;

            case Variant::objectType:
                return *a._data.t == *b._data.t;

            case Variant::valuelessType:
                return (a._type & Variant::typeMask) == (b._type & Variant::typeMask);
        }
        return false;
    } else {
        return false;
    }
}

bool operator!=(const Variant& a, const Variant& b)
{
    return !(a==b);
}

bool operator< (const Variant& a, const Variant& b)
{
    uint8_t aSuperType = a._type & Variant::superTypeMask;
    uint8_t bSuperType = b._type & Variant::superTypeMask;

    if (aSuperType == bSuperType) {
        switch (aSuperType) {
            case Variant::numericalType:
                {
                    bool aFloating = (a._type & Variant::typeMask) == Variant::floatingType;
                    bool bFloating = (b._type & Variant::typeMask) == Variant::floatingType;
                    if (aFloating && bFloating) {
                        return a._data.f < b._data.f;
                    } else if (aFloating) {
                        return a._data.f < b._data.l;
                    } else if (bFloating) {
                        return a._data.l < b._data.f;
                    } else {
                        return a._data.l < b._data.l;
                    }
                }

            case Variant::stringType:
                return *a._data.s < *b._data.s;

            case Variant::objectType:
                return *a._data.t < *b._data.t;
        }
        return false;
    } else {
        return aSuperType < bSuperType;
    }
}

bool operator<=(const Variant& a, const Variant& b)
{
    uint8_t aSuperType = a._type & Variant::superTypeMask;
    uint8_t bSuperType = b._type & Variant::superTypeMask;

    if (aSuperType == bSuperType) {
        switch (aSuperType) {
            case Variant::numericalType:
                {
                    bool aFloating = (a._type & Variant::typeMask) == Variant::floatingType;
                    bool bFloating = (b._type & Variant::typeMask) == Variant::floatingType;
                    if (aFloating && bFloating) {
                        return a._data.f <= b._data.f;
                    } else if (aFloating) {
                        return a._data.f <= b._data.l;
                    } else if (bFloating) {
                        return a._data.l <= b._data.f;
                    } else {
                        return a._data.l <= b._data.l;
                    }
                }

            case Variant::stringType:
                return *a._data.s <= *b._data.s;

            case Variant::objectType:
                return *a._data.t <= *b._data.t;
        }
        return true;
    } else {
        return aSuperType <= bSuperType;
    }
}

bool operator> (const Variant& a, const Variant& b)
{
    return !(a<=b);
}

bool operator>=(const Variant& a, const Variant& b)
{
    return !(a<b);
}


std::ostream& operator<<(std::ostream& out, const Variant& variant)
{
    return variant.display(out);
}

Variant &Variant::operator +=(const Variant &other)
{
    uint8_t superType = _type & superTypeMask;
    uint8_t otherSuperType = other._type & superTypeMask;
    if(superType == stringType || otherSuperType == stringType) {
        std::stringstream S;
        S<<*this<<other;
        setValue(S.str());
    } else if (superType == numericalType && otherSuperType == numericalType) {
        uint8_t type = _type & typeMask;

        if (type == floatingType) {
            _data.f += other.toDouble();
        } else if ((other._type & typeMask) == floatingType) {
            setValue(toDouble() + other._data.f);
        } else {
            _data.ul += other._data.ul;
        }
    }
    return *this;
}

Variant &Variant::operator -=(const Variant &other)
{
    uint8_t superType = _type & superTypeMask;
    uint8_t otherSuperType = other._type & superTypeMask;
    if (superType == numericalType && otherSuperType == numericalType) {
        uint8_t type = _type & typeMask;

        if (type == floatingType) {
            _data.f -= other.toDouble();
        } else if ((other._type & typeMask) == floatingType) {
            setValue(toDouble() - other._data.f);
        } else {
            if (type == unsignedIntegerType && _data.ul < other._data.ul) {
                _type |= 0x4;
            }
            _data.l -= other._data.l;
        }
    }
    return *this;
}

Variant &Variant::operator *=(const Variant &other)
{
    uint8_t superType = _type & superTypeMask;
    uint8_t otherSuperType = other._type & superTypeMask;
    if (superType == numericalType && otherSuperType == numericalType) {
        uint8_t type = _type & typeMask;

        if (type == floatingType) {
            _data.f *= other.toDouble();
        } else if ((other._type & typeMask) == floatingType) {
            setValue(toDouble()*other._data.f);
        } else {
            _data.l *= other._data.l;
        }
    }
    return *this;
}

Variant &Variant::operator /=(const Variant &other)
{
    uint8_t superType = _type & superTypeMask;
    uint8_t otherSuperType = other._type & superTypeMask;
    if (superType == numericalType && otherSuperType == numericalType) {
        uint8_t type = _type & typeMask;

        if (type == floatingType) {
            double otherValue = other.toDouble();
            if (otherValue != 0) {
                _data.f /= other.toDouble();
            } else {
                Log::error("Division by zero");
            }
        } else if ((other._type & typeMask) == floatingType) {
            if (other._data.f != 0) {
                setValue(toDouble() / other._data.f);
            } else {
                Log::error("Division by zero");
            }
        } else {
            if (other._data.l != 0) {
                _data.l /= other._data.l;
            } else {
                Log::error("Division by zero");
            }
        }
    }
    return *this;
}

Variant &Variant::operator %=(const Variant &other)
{
    uint8_t superType = _type & superTypeMask;
    uint8_t otherSuperType = other._type & superTypeMask;
    if (superType == numericalType && otherSuperType == numericalType) {
        uint8_t type = _type & typeMask;

        if (type == floatingType) {
            double otherValue = other.toDouble();
            if (otherValue != 0) {
                _data.f = fmod(_data.f, other.toDouble());
            } else {
                Log::error("Division by zero");
            }
        } else if ((other._type & typeMask) == floatingType) {
            if (other._data.f != 0) {
                setValue(fmod(toDouble(), other._data.f));
            } else {
                Log::error("Division by zero");
            }
        } else {
            if (other._data.l != 0) {
                _data.l %= other._data.l;
            } else {
                Log::error("Division by zero");
            }
        }
    }
    return *this;
}

Variant &Variant::operator ++()
{
    switch(_type & typeMask)
    {
        case integerType:
            ++_data.l;
            break;

        case unsignedIntegerType:
            ++_data.ul;
            break;

        case floatingType:
            ++_data.f;
            break;

        default:
            break;
    }
    return *this;
}

Variant Variant::operator ++(int)
{
    Variant result(*this);
    operator++();
    return result;
}

Variant &Variant::operator --()
{
    switch (_type & typeMask) {
        case integerType:
            --_data.l;
            break;

        case unsignedIntegerType:
            if(_data.ul == 0) {
                convertTo(integerType);
            }
            --_data.ul;

            break;

        case floatingType:
            --_data.f;
            break;

        default:
            break;
    }
    return *this;
}

Variant Variant::operator --(int)
{
    Variant result(*this);
    operator--();
    return result;
}

Variant Variant::operator -() const
{
    Variant result = *this;
    switch (_type & typeMask)
    {
        case unsignedIntegerType:
            result.convertTo(integerType);
        case integerType:
            result._data.l = -result._data.l;
            break;

        case floatingType:
            result._data.f = -result._data.f;
            break;
    }
    return result;
}

Variant &Variant::operator |=(const Variant &other)
{
    if ((_type & superTypeMask) == numericalType && (other._type & superTypeMask) == numericalType) {
        _data.ul |= other._data.ul;
    }
    return *this;
}

Variant &Variant::operator ^=(const Variant &other)
{
    if ((_type & superTypeMask) == numericalType && (other._type & superTypeMask) == numericalType) {
        _data.ul ^= other._data.ul;
    }
    return *this;
}

Variant &Variant::operator &=(const Variant &other)
{
    if ((_type & superTypeMask) == numericalType && (other._type & superTypeMask) == numericalType) {
        _data.ul &= other._data.ul;
    }
    return *this;
}

Variant &Variant::operator <<=(const Variant &other)
{
    if ((_type & superTypeMask) == numericalType && (other._type & superTypeMask) == numericalType) {
        _data.ul <<= other.toUnsignedInteger();
    }
    return *this;
}

Variant &Variant::operator >>=(const Variant &other)
{
    if ((_type & superTypeMask) == numericalType && (other._type & superTypeMask) == numericalType) {
        _data.ul >>= other.toUnsignedInteger();
    }
    return *this;
}

Variant Variant::operator ~() const
{
    if((_type & superTypeMask) == numericalType)
    {
        Variant result = *this;
        result._data.ul = ~result._data.ul;
        return result;
    } else {
        return Variant();
    }
}

bool Variant::operator !() const
{
    return !toBool();
}

bool operator&&(const Variant& a, const Variant& b)
{
    return a.toBool() && b.toBool();
}

bool operator||(const Variant& a, const Variant& b)
{
    return a.toBool() || b.toBool();
}

Variant operator+ (Variant a, const Variant& b)
{
    a += b;
    return a;
}

Variant operator- (Variant a, const Variant& b)
{
    a -= b;
    return a;
}

Variant operator* (Variant a, const Variant& b)
{
    a *= b;
    return a;
}

Variant operator/ (Variant a, const Variant& b)
{
    a /= b;
    return a;
}

Variant operator% (Variant a, const Variant& b)
{
    a %= b;
    return a;
}

Variant operator| (Variant a, const Variant& b)
{
    a |= b;
    return a;
}

Variant operator^ (Variant a, const Variant& b)
{
    a ^= b;
    return a;
}

Variant operator& (Variant a, const Variant& b)
{
    a &= b;
    return a;
}

Variant operator<<(Variant a, const Variant& b)
{
    a <<= b;
    return a;
}

Variant operator>>(Variant a, const Variant& b)
{
    a >>= b;
    return a;
}
