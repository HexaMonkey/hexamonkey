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

#ifndef LOCALSCOPE_H
#define LOCALSCOPE_H

#include <map>
#include <string>

#include "core/interpreter/scope/scope.h"

class Variable;

/**
 * @brief Scope implementation that allows for variables to be declared
 *
 * The keys can only be strings
 */
class LocalScope : public Scope
{
protected:
    virtual Variable doGet(const Variant& key) const;
    virtual Variable doDeclare(const Variant& key);

private:
    std::map<std::string, Variable> _map;
};

#endif // LOCALSCOPE_H
