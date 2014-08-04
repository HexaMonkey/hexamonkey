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

#ifndef VARIABLEDESCRIPTOR_H
#define VARIABLEDESCRIPTOR_H

#include <vector>
#include <ostream>

#include "core/variant.h"

/**
 * @brief Representation for a path giving access to a variable
 */
class VariablePath : public std::vector<Variant>
{
public:
    VariablePath();
    VariablePath(const std::vector<Variant>& other);
    VariablePath(std::initializer_list<Variant> l);

    bool inScopeOf(const VariablePath& other) const;
};


bool operator==(const VariablePath& a, const VariablePath& b);
bool operator!=(const VariablePath& a, const VariablePath& b);
bool operator< (const VariablePath& a, const VariablePath& b);
bool operator<=(const VariablePath& a, const VariablePath& b);
bool operator> (const VariablePath& a, const VariablePath& b);
bool operator>=(const VariablePath& a, const VariablePath& b);

std::ostream& operator << (std::ostream& out, const VariablePath& descriptor);

#endif // VARIABLEDESCRIPTOR_H
