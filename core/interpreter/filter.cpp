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

Filter::Filter(Interpreter* interpreter): _interpreter(interpreter)
{
}

bool Filter::setExpression(const std::string &expression)
{
    bool success = !expression.empty() && interpreter().loadFromString(expression);
    if(success)
    {
        _expression = expression;
        return true;
    }
    else
    {
        _expression = "";
        interpreter().clear();
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
        Holder holder(interpreter());
        return holder.cevaluate(interpreter().program().elem(0), ConstObjectScope(object, holder)).toBool();
    }
    else
        return true;
}

Interpreter &Filter::interpreter()
{
    return *_interpreter;
}
