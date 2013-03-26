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

#include "scope.h"

#include <string>
#include <vector>
#include <unordered_map>

class FunctionScope : public Scope
{
public:
    void addModifiableParameter(const std::string& name, Variant& variant);
    void addConstantParameter(const std::string& name, const Variant& variant);
protected:
    Variant* get(const Variant& key) const override;
    const Variant* cget(const Variant& key) const override;

private:
    unsigned int getIndex(const Variant& key) const;

    std::vector<Variant*> modifiableParameters;
    std::vector<const Variant*> constantParameters;

    std::unordered_map<std::string, int> parameterIndex;
};

#endif // FUNCTIONSCOPE_H
