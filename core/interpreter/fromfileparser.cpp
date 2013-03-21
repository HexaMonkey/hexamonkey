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

#include "fromfileparser.h"

#include "model.h"
#include "interpreter.h"
#include "variable.h"
#include "objecttypetemplate.h"

FromFileParser::FromFileParser(Object &object, const Module &module, Interpreter &interpreter, Program program, Program::const_iterator headerEnd)
    : ContainerParser(object, module),
      _interpreter(interpreter),
      _program(program),
      _index(program.begin()),
      _headerEnd(headerEnd),
      _objectScope(object)
{
    _scope.addScope(_localScope);
    _scope.addScope(_objectScope);
    do { (void)(hmcElemNames); } while (0);

}

void FromFileParser::doParseHead()
{
    int64_t fixedSize = module().getFixedSize(type());
    if(fixedSize > 0)
        setSize(fixedSize);

    _index = executeProgram(_index, _headerEnd);
}

void FromFileParser::doParse()
{
    _index = executeProgram(_index, _program.end());
    interpreter().garbageCollect();
    _localScope.clear();
}

bool FromFileParser::doParseSome(int hint)
{
    _index = executeProgram(_index, _program.end(), hint);
    if(_index == _program.end())
    {
        interpreter().garbageCollect();
        _localScope.clear();
        return true;
    }
    else
    {
        return false;
    }
}

Program::const_iterator FromFileParser::executeProgram(const Program::const_iterator &start, const Program::const_iterator &end, int64_t hint)
{

    Program::const_iterator current = start;
    for(int64_t i = 0;current != end && (hint == -1 || i < hint);++i)
    {
        const Program& line = *current;
        switch(line.id())
        {
            case DECLARATION:
                handleDeclaration(line);
                ++current;
                break;

            case LOCAL_DECLARATION:
                handleLocalDeclaration(line);
                ++current;
                break;

            case RIGHT_VALUE:
                handleRightValue(line);
                ++current;
                break;

            case CONDITIONAL_STATEMENT:
                handleCondition(line);
                ++current;
                break;

            case LOOP:
                if(handleLoop(line))
                    ++current;
                break;

            default:
                ++current;
                break;
        }
    }
    return current;
}

void FromFileParser::handleDeclaration(const Program &declaration)
{
    ObjectType type = interpreter().evaluate(declaration.elem(0), _scope, module()).value().toObjectType();
    std::string name = declaration.elem(1).payload().toString();
    addVariable(type, name);
}

void FromFileParser::handleLocalDeclaration(const Program &declaration)
{
    Variant* variant = _scope.declare(declaration.elem(0).payload());
    if(declaration.size() >= 2 && variant != nullptr)
    {
        Variable& right = interpreter().evaluate(declaration.elem(1), _scope, module());
        *variant = right.value();
        interpreter().release(right);
    }
}

void FromFileParser::handleRightValue(const Program &rightValue)
{
    interpreter().release(interpreter().evaluate(rightValue, _scope, module()));
}

void FromFileParser::handleCondition(const Program &condition)
{
    Variable& choice = interpreter().evaluate(condition.elem(0), _scope);
    if(choice.value().toBool())
    {
        executeProgram(condition.elem(1).begin(), condition.elem(1).end());
    }
    else
    {
        executeProgram(condition.elem(2).begin(), condition.elem(2).end());
    }
    interpreter().release(choice);
}

bool FromFileParser::handleLoop(const Program &loop)
{
    if(interpreter().hasDeclaration(loop.elem(1)) && availableSize()<= 0)
        return true;

    Variable& choice = interpreter().evaluate(loop.elem(0), _scope);
    if(!choice.value().toBool())
    {
        interpreter().release(choice);
        return true;
    }
    interpreter().release(choice);

    executeProgram(loop.elem(1).begin(), loop.elem(1).end());
    return false;
}

Interpreter &FromFileParser::interpreter()
{
    return _interpreter;
}
