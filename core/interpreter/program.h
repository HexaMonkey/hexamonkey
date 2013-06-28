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

#include "object.h"
#include "module.h"
#include "variabledescriptor.h"


#include <memory>
#include <set>

class Interpreter;


class Program
{
    class Memory
    {
        friend class Interpreter;
        friend class Program;

        Variable& registerVariable(Variable* variable);
        void releaseVariable(Variable& variable);
        File& file();
        Object& setFileObject(Object* fileObject);

        File _file;
        std::unique_ptr<Object> _fileObject;
        std::map<Variable*, std::unique_ptr<Variable> > _variables;
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

    bool isValid() const;
    uint32_t id() const;
    const Variant& payload() const;
    int size() const;
    Program elem(int index) const;

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

    ObjectType evaluateType(const Scope& scope, const Module& module) const;
    bool hasDeclaration() const;

    void buildDependencies(bool modificationOnly, std::set<VariableDescriptor>& descriptors) const;

    int64_t guessSize(const Module& module) const;

private:
    friend class Interpreter;
    friend class Holder;
    friend class BlockExecution;
    Program(Object& object, std::shared_ptr<Memory> memory);

    Variable& copy(const Variant& value) const;
    Variable& constReference(const Variant& value) const;
    Variable& reference(Variant& value) const;
    Variable& null() const;
    Variable& registerVariable(Variable* variable) const;
    void releaseVariable(Variable& variable) const;

    Variable& evaluate(const Scope& scope, const Module& module = Module()) const;

    Variable& evaluateUnaryOperation(int op, Variable& a) const;
    Variable& evaluateBinaryOperation(int op, Variable& a, Variable& b) const;
    Variable& evaluateTernaryOperation(int op, Variable& a, Variable& b, Variable& c) const;
    Variable& evaluateFunction(const Scope& scope, const Module& module) const;
    Variable& evaluateVariable(const Scope& scope, const Module& module = Module()) const;

    void buildVariableDescriptor(const Scope& scope, const Module& module, VariableDescriptor& variableDescriptor) const;

    Program::Memory& memory() const;

    Object* _object;
    std::shared_ptr<Program::Memory> _memory;
};

#endif // EBMLOBJECT_H
