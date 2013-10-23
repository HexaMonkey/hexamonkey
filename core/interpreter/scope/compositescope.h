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

#ifndef COMPOSITESCOPE_H
#define COMPOSITESCOPE_H

#include <vector>

#include "core/interpreter/scope/scope.h"

/**
 * @brief Scope implementation that combines other scopes.
 *
 * When a request is made the scopes added are enqueried
 * consecutively in the order that they have been added
 * until a query is successful in which case the response
 * of the scope is transferred.
 *
 */
class CompositeScope : public Scope
{
public:
    /**
     * @brief Add a scope without owning the memory
     */
    void addScope(Scope& scope);
    /**
     * @brief Add a scope without and owns the memory
     */
    void addScope(Scope* scope);

protected:
    Variable doGet(const Variant &key) const override;
    Ptr const doGetScope(const Variant &key) const override;
    Variable doDeclare(const Variant &key) override;
private:
    std::vector<Scope::Ptr> _scopes;
};

#endif // COMPOSITESCOPE_H
