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

#ifndef EBMLOBJECT_H
#define EBMLOBJECT_H

#include <memory>
#include <set>

#include "core/object.h"
#include "core/module.h"
#include "core/interpreter/variablepath.h"

/**
 * @brief Node of the abstract syntaxing tree of an HmScript file
 *
 * The root of a \link Program program\endlink can be loaded by the
 * \link ProgramLoader program loader\endlink. The children nodes can then be generated
 * by iterating over the node or accessing them by their index. Leaf nodes
 * and memory of the whole tree is shared by all the nodes generated.
 */
class Program
{
    class Memory
    {
        friend class ProgramLoader;
        friend class Program;

        File& file();
        Object& setFileObject(Object* fileObject);

        File _file;
        std::unique_ptr<Object> _fileObject;
    };

    template<class It>
    class _const_iterator : public std::iterator<std::input_iterator_tag, Program>
    {
        friend class Program;
        It _it;
        std::shared_ptr<Memory> _memory;

        _const_iterator<It>(const It& it, std::shared_ptr<Memory> memory):_it(it), _memory(memory){}
        public:
            _const_iterator<It>(){}
            _const_iterator<It>& operator++() {++_it; return *this;}
            _const_iterator<It> operator++(int) {const_iterator dup(*this); ++_it; return dup;}
            _const_iterator<It>& operator--() {--_it; return *this;}
            _const_iterator<It> operator--(int) {const_iterator dup(*this); --_it; return dup;}
            Program operator*() const {return Program(**_it, _memory);}
            bool operator==(const _const_iterator<It>& other) const {return _it==other._it;}
            bool operator!=(const _const_iterator<It>& other) const {return !(*this==other);}
    };

public:
    typedef _const_iterator< Object::iterator > const_iterator;
    typedef _const_iterator< Object::reverse_iterator > const_reverse_iterator;

    Program();

    /**
     * @brief Check if the program has been successfully loaded
     */
    bool isValid() const;
    /**
     * @brief Get the type representing the type of the node.
     *
     * The macros for the tags are defined in model.h
     */
    uint32_t tag() const;

    /**
     * @brief Get the value of the leaf node
     *
     * The behaviour is undefined if the node is not a leaf
     */
    const Variant& payload() const;

    /**
     * @brief Get the number of children node
     */
    int size() const;

    /**
     * @brief Get a child node by its index
     *
     * Returns an invalid \link Program program\endlink if out of bound
     */
    Program node(int index) const;

    /**
    * @brief Get a child node by its index
     *
     * Returns an invalid \link Program program\endlink if out of bound
     */
    Program operator[](int index) const;

    /**
     * @brief Get an iterator to the beginning of the children nodes
     */
    const_iterator begin() const;

    /**
     * @brief Get an iterator to the ending of the children nodes
     */
    const_iterator end() const;

    /**
     * @brief Get a reverse iterator to the beginning of the children nodes
     */
    const_reverse_iterator rbegin() const;

    /**
     * @brief Get a reverse iterator to the ending of the children nodes
     */
    const_reverse_iterator rend() const;

private:
    friend class ProgramLoader;

    Program(Object& object, std::shared_ptr<Memory> memory);

    Program::Memory& memory() const;

    Object* _object;
    std::shared_ptr<Program::Memory> _memory;
};

#endif // EBMLOBJECT_H
