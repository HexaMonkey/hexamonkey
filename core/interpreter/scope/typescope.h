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

#ifndef TYPESCOPE_H
#define TYPESCOPE_H

#include "core/interpreter/scope/scope.h"

class Parser;
class ObjectType;
/**
 * @brief Scope implementation that gives access to a \link ObjectType type\endlink's parameters
 *
 * The arguments can either be access by their index or name.
 */
class AbstractTypeScope : public Scope
{
protected:
    virtual Variable doGet(const Variant &key, bool modifiable) override;
    virtual Variable getValue(bool) override;

    virtual ObjectType* modifiableType() = 0;
    virtual const ObjectType& constType() = 0;
};

/**
 * @brief Type scope implementation given a reference to a \link ObjectType type\endlink
 */
class TypeScope : public AbstractTypeScope
{
public:
    TypeScope(ObjectType& type, bool modifiable);
    TypeScope(const ObjectType& type);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    ObjectType* _type;
    const ObjectType& _constType;
};

/**
 * @brief Type scope implementation given a reference to a \link Parser parser\endlink
 */
class ParserTypeScope : public AbstractTypeScope
{
public:
    ParserTypeScope(Parser& parser);
protected:
    virtual ObjectType* modifiableType() override;
    virtual const ObjectType& constType() override;
private:
    Parser& _parser;
};


#endif // TYPESCOPE_H
