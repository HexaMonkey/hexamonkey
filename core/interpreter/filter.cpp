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

#include "core/object.h"
#include "core/parser.h"
#include "core/objecttypetemplate.h"
#include "core/interpreter/filter.h"
#include "core/interpreter/programloader.h"
#include "core/variable/variable.h"
#include "core/variable/objectscope.h"

Filter::Filter(const ProgramLoader& programLoader): _programLoader(programLoader)
{
}

bool Filter::setExpression(const std::string &expression)
{
    if(expression.empty())
    {
        _expression = "";
        return false;
    }

    _program = _programLoader.fromString(expression);

    if(_program.isValid() && _program.size() > 0)
    {
        _program = _program.node(0);
        if(_program.isValid())
        {
            _expression = expression;
            return true;
        }
    }

    _expression = "";
    return false;
}

const std::string &Filter::expression()
{
    return _expression;
}

bool Filter::operator()(Object& object)
{
    if(_expression != "") {
        Variable objectVariable = object.variable();
        objectVariable.setConstant();
        return Evaluator(objectVariable).rightValue(_program).value().toBool();
    } else {
        return true;
    }
}

