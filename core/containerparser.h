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

#ifndef CONTAINERPARSER_H
#define CONTAINERPARSER_H

#include <functional>

#include "core/parser.h"
#include "core/parsingexception.h"

class Module;

/*!
 * @brief Parser able to add children to the object
 */
class ContainerParser : public Parser
{
public:

    virtual ~ContainerParser() {}

    /**
     * @brief Generate an \link Object object\endlink not to be subsequently added
     */
    Object* readVariable(const ObjectType& type, std::streamoff offset = 0);

    /**
     * @brief Generate an \link Object object\endlink and add it
     */
    Object* addVariable(const ObjectType& type);

    /**
     * @brief Generate an \link Object object\endlink, set its name, and add it
     */
    Object* addVariable(const ObjectType& type, const std::string& name);

    const Module& module() const;

protected:
    ContainerParser(Object& object, const Module& module);
    ContainerParser(ParsingOption& option, const Module& module);

private:
    /**
     * @brief Generate an \link Object object\endlink to be subsequently added (or not)
     */
    Object* getVariable(const ObjectType& type, std::streamoff offset = 0);

    const Module& _module;
};

#endif // CONTAINERPARSER_H
