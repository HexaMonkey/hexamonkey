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

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <set>
#include <memory>

#include <stdint.h>

#include "file.h"
#include "objecttype.h"
#include "module.h"
#include "program.h"

class Object;
class HmcModule;
class Variable;
class Scope;
class VariableDescriptor;

class Interpreter
{
public:
    enum Mode {file, expression};
    ~Interpreter();

    bool loadFromString(const std::string &exp);
    bool loadFromHM (const std::string& path, int mode);
    bool loadFromHMC(const std::string& path);
    const std::string& error() const;

    void setObject(Object &object);

    Program& program();

    Variable& evaluate(const Program& rightValue, const Scope& scope, const Module& module = Module());
    Variable& evaluateBinaryOperation(int op, Variable& a, Variable& b);
    Variable& evaluateUnaryOperation(int op, Variable& a);
    Variable& evaluateTernaryOperation(int op, Variable& a, Variable& b, Variable& c);
    Variable& evaluateVariable(const Program& variable, const Scope& scope, const Module& module = Module());
    ObjectType evaluateType(const Program& type, const Scope& scope, const Module& module);
    bool hasDeclaration(const Program& classDefinition);

    void buildDependencies(const Program& program, bool modificationOnly, std::set<VariableDescriptor>& descriptors);

    int64_t guessSize(const Program& classDefinition, const Module& module);

    Variable& copy(const Variant& value);
    Variable& constReference(const Variant& value);
    Variable& reference(Variant& value);

    void release(Variable& var);
    void garbageCollect();
    void clear();



private:
    friend class InterpreterConstructor;
    Interpreter(const HmcModule& module);
    Interpreter(const Interpreter& interpreter) = delete;
    Interpreter& operator =(const Interpreter& interpreter) = delete;
    void buildVariableDescriptor(const Program& variable, const Scope& scope, const Module& module, VariableDescriptor& variableDescriptor);

    const HmcModule& _module;
    Program* _program;
    Object* _currentObject;

    File _file;
    Object* _fileObject;

    const Variant null;
    const Variant emptyString;

    std::set<Variable*> _vars;

    std::string _error;
};

#endif // INTERPRETER_H
