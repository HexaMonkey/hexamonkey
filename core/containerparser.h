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

#include "parser.h"

class Module;

/*!
 * \brief The ContainerParser class
 */
class ContainerParser : public Parser
{
public:
    ContainerParser(Object& object, const Module& module);
    virtual ~ContainerParser() {}

    void addChild(Object* child);
    void addChild(Object* child, const std::string& name);

    Object* getVariable(const ObjectType& type);
    Object* addVariable(const ObjectType& type);
    Object* addVariable(const ObjectType& type, const std::string& name);

    void addLoop(const std::function<void(ContainerParser&)>& body, const std::string& name = "");
    void addLoop(const std::function<bool()>& guard, const std::function<void(ContainerParser&)>& body, const std::string& name = "");
    void addForLoop(int64_t max, const std::function<void(ContainerParser&, int64_t)>& body, const std::string& name = "");

    const Module& module() const;

protected:
    static ContainerParser& containerParser(Object& object);
    void setAutogrow();

private:
    const Module& _module;
    bool _autogrow;
};

#endif // CONTAINERPARSER_H
