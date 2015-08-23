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
#include <unordered_map>
#include <functional>

#include "core/objecttype.h"
#include "core/variant.h"


#define objectTypeAttributeLambda (const ObjectType &type) ->Variant

/**
 * @brief Template for a \link ObjectType type\endlink
 *
 * A \link ObjectTypeTemplate type template\endlink is defined by a unique
 * name and a number of named parameters
 */
class ObjectTypeTemplate
{
public:
    enum class Attribute{
        name = 0,
        elementType = 1,
        elementCount = 2,
        displayMode = 3,
        displayAs = 4
    };

    typedef std::function<Variant (const ObjectType &)> AttributeGenerator;

    ObjectTypeTemplate(const std::string &name,
                       const std::vector<std::string>& parameterNames,
                       const std::function<void (ObjectTypeTemplate&)> initialization);
    ObjectTypeTemplate(const std::string &name,
                       const std::vector<std::string>& parameterNames);
    ObjectTypeTemplate(const std::string &name);

    /**
     * @brief Get the name used as a unique identifiers
     */
    const std::string& name() const;

    /**
     * @brief Get the number of parameters
     */
    int numberOfParameters() const;

    /**
     * @brief Get the name of a parameter given its name
     *
     * Raise an exception if out of bounds
     */
    const std::string& parameterName(int index) const;

    /**
     * @brief Check if the parameter is private
     *
     * If a paramater is private it should not be displayed on screen
     *
     * Raise an exception if out of bounds
     */
    bool isParameterPrivate(int index) const;

    /**
     * @brief Get the index of a parameter given its name
     *
     * Returns -1 if no parameter of this name exists
     */
    int parameterNumber(const std::string& name) const;

    /**
     * @brief Check if the template has the name "". \link ObjectType Types\endlink when constructed
     * by default have a null \link ObjectTypeTemplate template\endlink.
     */
    bool isNull() const;

    /**
     * @brief Push back a new parameter
     */
    void addParameter(const std::string& parameterName);

    /**
     * @brief Instanciate a \link ObjectType type\endlink with the
     * \link ObjectTypeTemplate template\endlink and set values for a number of its parameters
     */
    template<typename... Args> ObjectType operator()(Args... args) const
    {
        ObjectType type(*this);
        type.setParameters(args...);
        return type;
    }

    void setAttributeGenerator(Attribute attribute, const AttributeGenerator& generator);
    bool hasAttributeGenerator(Attribute attribute) const;
    const AttributeGenerator &attributeGenerator(Attribute attribute) const;

    bool isVirtual() const;
    void setVirtual(bool value);

    friend bool operator==(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);
    friend bool operator< (const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);

private:
    std::string                _name;
    std::vector<std::string>   _parametersNames;
    std::unordered_map<std::string, int> _parametersNumbers;

    const static uint8_t _nameAttribute;
    const static uint8_t _elementTypeAttribute;
    const static uint8_t _elementCountAttribute;

    uint8_t _attributeFlag;
    std::vector<AttributeGenerator> _attributeGenerators;

    AttributeGenerator _nameGenerator;
    AttributeGenerator _elementTypeGenerator;
    AttributeGenerator _elementCountGenerator;


    bool _virtual;

    ObjectTypeTemplate(const ObjectTypeTemplate&) = delete;
    ObjectTypeTemplate& operator=(const ObjectTypeTemplate&) = delete;
};

const ObjectTypeTemplate nullTypeTemplate("");

bool operator!=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);
bool operator<=(const ObjectTypeTemplate& a, const ObjectTypeTemplate& b);

#endif // OBJECTTYPETEMPLATE_H
