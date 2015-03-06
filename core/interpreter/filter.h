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

#ifndef FILTER_H
#define FILTER_H

#include <memory>
#include <string>

#include "core/parser.h"
#include "core/interpreter/programloader.h"
#include "core/variable/variable.h"
#include "core/interpreter/evaluator.h"

class Object;

/**
 * @brief Evaluate an HMScript statement on an object
 */
class Filter
{
public:
    Filter(const ProgramLoader &_programLoader);

    bool setExpression(const std::string& expression);
    const std::string& expression();

    bool operator()(Object &object);

private:
    const ProgramLoader& _programLoader;
    Program _program;
    std::string _expression;
};

#endif // FILTER_H
