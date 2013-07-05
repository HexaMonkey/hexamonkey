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

#include <memory>

#include "containerparser.h"

#include "program.h"

#include "objectscope.h"
#include "localscope.h"
#include "compositescope.h"
#include "blockexecution.h"
#include "evaluator.h"

/**
 * @brief Parser implementation using an HMScript class definition
 *
 * Uses an instance of BlockExecution to execute the program.
 * A breakpoint is given to isolate the head, which is computed
 * statically by the module.
 */
class FromFileParser : public ContainerParser
{
public:
    FromFileParser(Object& object, const Module &module, Program classDefinition, Program::const_iterator headerEnd);

private:
    virtual void doParseHead() final;
    virtual void doParse() final;
    virtual bool doParseSome(int hint) final;

    Scope &scope();
    BlockExecution& blockExecution();
    Program::const_iterator _headerEnd;

    ObjectScope _objectScope;
    LocalScope _localScope;
    CompositeScope _scope;

    Evaluator _evaluator;

    BlockExecution _blockExecution;

    Object& _object;

};

#endif // FROMFILEPARSER_H
