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

#include "compiler/model.h"
#include "core/objecttypetemplate.h"
#include "core/interpreter/fromfileparser.h"
#include "core/interpreter/programloader.h"
#include "core/interpreter/variable.h"
#include "core/util/unused.h"

FromFileParser::FromFileParser(Object &object, const Module &module, Program classDefinition, Program::const_iterator headerEnd)
    : ContainerParser(object, module),
      _headerEnd(headerEnd),
      _objectScope(object, true),
      _evaluator(_scope, module),
      _bodyExecution(classDefinition.node(0), _evaluator, _scope, this),
      _tailExecution(classDefinition.node(1), _evaluator, _scope, this),
      _object(object)
{
    _scope.addScope(_localScope);
    _scope.addScope(_objectScope);
    UNUSED(hmcElemNames);

    if(module.getFixedSize(type()) == -1 && headerEnd == classDefinition.begin())
    {
        setNoHead();
    }
}

void FromFileParser::doParseHead()
{
    int64_t fixedSize = module().getFixedSize(type());
    if(fixedSize > 0)
        setSize(fixedSize);

    _bodyExecution.execute(_headerEnd);
    if(_bodyExecution.done()) {
        setParsed();
    }
}

void FromFileParser::doParse()
{
    _bodyExecution.execute();
}

bool FromFileParser::doParseSome(int hint)
{
    size_t parseQuota = hint;
    _bodyExecution.execute(parseQuota);
    if(_bodyExecution.done())
        return true;
    return false;
}

void FromFileParser::doParseTail()
{
    _tailExecution.execute();
}

