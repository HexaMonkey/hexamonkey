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

#ifndef FROMFILEPARSER_H
#define FROMFILEPARSER_H

#include "containerparser.h"

#include "program.h"

#include "objectscope.h"
#include "localscope.h"
#include "compositescope.h"

class Interpreter;

class FromFileParser : public ContainerParser
{
public:
    FromFileParser(Object& object, const Module& module, Interpreter& interpreter, Program program, Program::const_iterator headerEnd);

protected:
    void doParseHead() override;
    void doParse() override;
    bool doParseSome(int hint) override;

private:
    Program::const_iterator executeProgram(const Program::const_iterator& start, const Program::const_iterator& end, int64_t hint = -1);
    void handleDeclaration(const Program& declaration);
    void handleLocalDeclaration(const Program& declaration);
    void handleRightValue(const Program& rightValue);
    void handleCondition(const Program& condition);
    bool handleLoop(const Program& loop);

    Interpreter& interpreter();

    Interpreter& _interpreter;
    Program _program;
    Program::const_iterator _index;
    Program::const_iterator _headerEnd;

    MutableObjectScope _objectScope;
    LocalScope _localScope;
    CompositeScope _scope;

};

#endif // FROMFILEPARSER_H
