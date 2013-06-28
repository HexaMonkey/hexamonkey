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

#include "filter.h"
#include "object.h"
#include "objecttypetemplate.h"
#include "objectscope.h"
#include "interpreter.h"
#include "variable.h"
#include "parser.h"
#include "holder.h"

Filter::Filter(const Interpreter& interpreter): _interpreter(interpreter)
{
}

bool Filter::setExpression(const std::string &expression)
{
    if(expression.empty())
    {
        _expression = "";
        return false;
    }

    _program = _interpreter.loadFromString(expression);

    if(_program.isValid())
    {
        _expression = expression;
        return true;
    }
    else
    {
        _expression = "";
        return false;
    }
}

const std::string &Filter::expression()
{
    return _expression;
}

bool Filter::filterChildren(Object& object)
{

    if(_expression != "")
    {
        Holder holder(_program.elem(0));
        return holder.cevaluate(ConstObjectScope(object, holder)).toBool();
    }
    else
        return true;
}

