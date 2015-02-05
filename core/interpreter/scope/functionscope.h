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

#ifndef FUNCTIONSCOPE_H
#define FUNCTIONSCOPE_H

#include <string>
#include <vector>
#include <unordered_map>

#include "core/interpreter/scope/scope.h"

/**
 * @brief Scope implementation that represents gives access to a
 * function's parameters
 *
 * The parameter can either by its index or its name
 */
class FunctionScope : public Scope
{
public:
    void addParameter(const std::string& name, Variable variable);
protected:
    virtual Variable doGet(const Variant& key, bool modifiable) override;

private:
    unsigned int getIndex(const Variant& key) const;

    std::vector<Variable> parameters;

    std::unordered_map<std::string, int> parameterIndex;
};

#endif // FUNCTIONSCOPE_H
