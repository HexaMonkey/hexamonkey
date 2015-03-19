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

#ifndef VARIANT_H
#define VARIANT_H

#include <exception>
#include <iostream>
#include <string>

class ObjectType;

/*!
 * @brief Tagged union for holding values
 *
 * Can hold: 64 bits signed integers (integer), 64 bits unsigned integers (unsignedInteger),
 * 64 bits floating points (floating), strings (string) and \link ObjectType object types\endlink (objectType).
 * By default the \link Variant variant\endlink's value is not defined and the type is unknown, in which case
 * most operations will fail, and the value is value is null.
 *
 * Explicit conversion are allowed in some cases : you can check if a conversion is possible
 * using canConvertTo and then convert the internal value using convertTo.
 *
 * Most usual operators have been overloaded and behave as exepected is c/c++
 */
class Variant
{
public:
    enum Type{
        ///Default supertype
        valuelessType       = 0x00,
        ///Undefined variant
        undefinedType       = 0x00,
        ///Defined variant with no value
        nullType            = 0x04,
        ///Internal supertype for integer, unsigned integer and floating
        numericalType       = 0x01,
        ///64 bits signed integer, conversion with other numerical
        ///types possible (ie unsignedInteger and floating)
        integerType         = 0x05,
        ///64 bits unsigned integer, conversion with other numerical
        ///types possible (ie integer and floating)
        unsignedIntegerType = 0x01,
        ///64 bits floating point, conversion with other numerical
        ///types possible (ie integer and unsignedInteger)
        floatingType        = 0x09,
        ///std::string, no conversion possible
        stringType          = 0x02,
        ///Objectype, no conversion possible
        objectType          = 0x03
    };

    enum Display{
        decimal     = 0x00,
        binary      = 0x10,
        octal       = 0x20,
        hexadecimal = 0x30
    };

    /// @cond HIDDEN_SYMBOL
    class InvalidTypeConversionException: public std::exception
    {
      virtual const char* what() const throw()
      {
        return "Variant: invalid type conversion";
      }
    };

    class InvalidOperationException: public std::exception
    {
      virtual const char* what() const throw()
      {
        return "Variant: invalid operation";
      }
    };
    /// @endcond

    Variant();
    static Variant null();

    Variant(const Variant& other);
    Variant(Variant&& other);

    Variant(bool l);
    Variant(char l);
    Variant(int l);
    Variant(long l);
    Variant(long long l);

    Variant(unsigned char ul);
    Variant(unsigned int ul);
    Variant(unsigned long ul);
    Variant(unsigned long long ul);

    Variant(float f);
    Variant(double f);

    Variant(const std::string& s);
    Variant(const char* s);

    Variant(const ObjectType& t);

    ~Variant();

    void setValue(Variant other);

    void setValue(bool l);
    void setValue(char l);
    void setValue(int l);
    void setValue(long l);
    void setValue(long long l);

    void setValue(unsigned char ul);
    void setValue(unsigned int ul);
    void setValue(unsigned long ul);
    void setValue(unsigned long long ul);

    void setValue(float f);
    void setValue(double f);

    void setValue(const std::string& s);
    void setValue(const char* s);

    void setValue(const ObjectType& t);

    void clear();

    Variant& operator=(Variant other);
    friend void swap(Variant& a, Variant& b);

    bool canConvertTo(Type otherType) const;
    Variant &convertTo(Type newType);

    long long          toInteger()         const;
    unsigned long long toUnsignedInteger() const;
    double             toDouble()          const;
    const std::string& toString()          const;
    const ObjectType&  toObjectType()      const;
    bool               toBool()            const;

    Type type() const;
    bool hasNumericalType() const;
    bool isValueless() const;
    bool isNull() const;
    bool isUndefined() const;

    friend bool operator==(const Variant& a, const Variant& b);
    friend bool operator< (const Variant& a, const Variant& b);
    friend bool operator<=(const Variant& a, const Variant& b);

    Variant& operator+=(const Variant& other);
    Variant& operator-=(const Variant& other);
    Variant& operator*=(const Variant& other);
    Variant& operator/=(const Variant& other);
    Variant& operator%=(const Variant& other);

    Variant& operator++();
    Variant  operator++(int);
    Variant& operator--();
    Variant  operator--(int);


    Variant operator-() const;

    Variant& operator|=(const Variant& other);
    Variant& operator^=(const Variant& other);
    Variant& operator&=(const Variant& other);
    Variant& operator<<=(const Variant& other);
    Variant& operator>>=(const Variant& other);

    Variant operator~() const;

    bool operator!() const;

    void setDisplayType(Display display);
    std::ostream& display(std::ostream& out, bool setFlags = true) const;



private:
    typedef union{
        long long l;
        unsigned long long ul;
        double f;
        std::pair<std::string, int>* s;
        std::pair<ObjectType, int>* t;
    } Data;

    Data    _data;
    uint8_t _type;

    static const uint8_t superTypeMask = 0x03;
    static const uint8_t typeMask = 0x0f;
    static const uint8_t displayMask = 0x30;
    static const uint8_t signedBit = 0x04;
};

void swap(Variant& a, Variant& b);

Variant operator+ (Variant a, const Variant& b);
Variant operator- (Variant a, const Variant& b);
Variant operator* (Variant a, const Variant& b);
Variant operator/ (Variant a, const Variant& b);
Variant operator% (Variant a, const Variant& b);

Variant operator| (Variant a, const Variant& b);
Variant operator^ (Variant a, const Variant& b);
Variant operator& (Variant a, const Variant& b);
Variant operator<<(Variant a, const Variant& b);
Variant operator>>(Variant a, const Variant& b);

bool operator==(const Variant& a, const Variant& b);
bool operator!=(const Variant& a, const Variant& b);
bool operator< (const Variant& a, const Variant& b);
bool operator<=(const Variant& a, const Variant& b);
bool operator> (const Variant& a, const Variant& b);
bool operator>=(const Variant& a, const Variant& b);

bool operator&&(const Variant& a, const Variant& b);
bool operator||(const Variant& a, const Variant& b);

std::ostream& operator<<(std::ostream& out, const Variant& variant);

/** @cond */
namespace std
{
    template<>
    struct hash<Variant>
    {
        std::size_t operator()(Variant const& var) const
        {
            std::size_t const h1 ( std::hash<int>()(var.type()) );
            std::size_t const h2 ( std::hash<std::string>()(var.toString()) );
            return h1 ^ (h2 << 3);
        }
    };
}
/** @endcond */

#endif // VARIANT_H
