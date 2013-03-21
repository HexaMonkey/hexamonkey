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

#ifndef OBJECTTYPETEMPLATE_H
#define OBJECTTYPETEMPLATE_H

#include <string>
#include <vector>
#include <map>

#include "variant.h"
#include "objecttype.h"

/*!
 * \brief The ObjectTypeTemplate class
 */
class ObjectTypeTemplate
{
public:
    ObjectTypeTemplate(const std::string &name, const std::vector<std::string>& parameterNames);
    ObjectTypeTemplate(const std::string &name);

    const std::string& name() const;
    int numberOfParameters() const;
    const std::string& parameterName(int index) const;
    int parameterNumber(const std::string& name) const;
    bool isNull() const;

    void addParameter(const std::string& parameterName);

    template<typename... Args> ObjectType operator()(Args... args) const
    {
        ObjectType type(*this);
        type.setParameters(args...);
        return type;
    }
    friend bool operator==(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);
    friend bool operator< (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);

private:
    std::string                _name;
    std::vector<std::string>   _parametersNames;
    std::map<std::string, int>      _parametersNumbers;

    ObjectTypeTemplate(const ObjectTypeTemplate&) = delete;
    ObjectTypeTemplate& operator=(const ObjectTypeTemplate&) = delete;
};

const ObjectTypeTemplate nullTypeTemplate("");

bool operator!=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);
bool operator<=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);

#endif // OBJECTTYPETEMPLATE_H
