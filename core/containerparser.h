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

class Module;

/*!
 * @brief Parser able to add children to the object
 */
class ContainerParser : public Parser
{
public:

    virtual ~ContainerParser() {}

    /**
     * @brief Add a child to the \link Object object\endlink
     * 
     * The object will be in charge of managing the child's memory
     */
    void addChild(Object* child);

    /**
     * @brief Add a child and set its name
     */
    void addChild(Object* child, const std::string& name);

    /**
     * @brief Generate an \link Object object\endlink to be subsequently added (or not)
     */
    Object* getVariable(const ObjectType& type);

    /**
     * @brief Generate an \link Object object\endlink and add it
     */
    Object* addVariable(const ObjectType& type);

    /**
     * @brief Generate an \link Object object\endlink, set its name, and add it
     */
    Object* addVariable(const ObjectType& type, const std::string& name);

    /**
     * @brief Add an \link Object object\endlink, with a parser executing
     * a function while there is still available space
     */
    void addLoop(const std::function<void(ContainerParser&)>& body, const std::string& name = "");

    /**
     * @brief Add an \link Object object\endlink, with a parser executing
     * a function while there is still available space and the guard return true
     */
    void addLoop(const std::function<bool()>& guard, const std::function<void(ContainerParser&)>& body, const std::string& name = "");

    /**
     * @brief Add an \link Object object\endlink, with a parser executing
     * a function while there is still available space and the function has been
     * executed less than max times
     */
    void addForLoop(int64_t max, const std::function<void(ContainerParser&, int64_t)>& body, const std::string& name = "");

    const Module& module() const;

protected:
    ContainerParser(Object& object, const Module& module);

    /**
     * @brief Insure that when children are added past the end of the object that its
     * size grows automaticaly
     */
    void setAutogrow();

private:
    const Module& _module;
    bool _autogrow;
};

#endif // CONTAINERPARSER_H
