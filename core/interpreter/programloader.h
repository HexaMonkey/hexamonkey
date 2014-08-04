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

#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include <set>
#include <memory>
#include <stdint.h>

#include "core/file/file.h"
#include "core/objecttype.h"
#include "core/module.h"
#include "core/interpreter/program.h"

class HmcModule;

/**
 * @brief Compile and load \link Program programs\endlink
 *
 * The \link ProgramLoader program loader\endlink can load a \link Program program\endlink
 * from a compiled HmScript file to be used as a \link Module module\endlink : FromFileModule. It can also
 * compile a HmScript file in order to load subsequently the compiled version.
 *
 * The program loader can also load a \link Program program\endlink from a string compiled by expcompiler
 * to be used as a right value that can be evaluated. This is for instance used by \link Filter filters\endlink
 * to evaluate if an \link Object object\endlink should pass filtering test or not.
 */
class ProgramLoader
{
public:
    /**
     * @param module used to parse compiled HMScript files
     */
    ProgramLoader(const HmcModule &module, const std::vector<std::string> &compilerDirs, const std::string userDir);

    /**
     * @brief Load a \link Program program\endlink from a string to be used as a right value.
     */
    Program fromString(const std::string &exp) const;

    /**
     * @brief Compile and load a \link Program program\endlink from an HmScript file to be
     * used as a \link Module module\endlink.
     */
    Program fromHM (const std::string& path) const;

    /**
     * @brief Compile \link Program program\endlink from a compiled HmScript file to be
     * used as a \link Module module\endlink.
     */
    Program fromHMC(const std::string& path) const;

    /**
     * @brief Load a \link Program program\endlink from the appopriate source to be used as a module.
     *
     * The basePath is the name of the HmScript file stripped of its extension. The function will either
     * use the HmScript file or the compiled Hmscript file, depending on their existences and their last
     * modification times.
     */
    Program fromFile(const std::string& basePath) const;

protected:
    virtual bool executeCommand(const std::string& program, const std::vector<std::string>& arguments) const;
    virtual long long lastModified(const std::string& file) const;

private:
    enum Mode {file, expression};
    Program fromHM (const std::string& path, int mode) const;

    const HmcModule& _module;
    const std::string _fileCompiler;
    const std::string _expCompiler;
    const std::string _userDir;

};

#endif // PROGRAM_LOADER_H
