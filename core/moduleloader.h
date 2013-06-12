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

#ifndef MODULELOADER_H
#define MODULELOADER_H

#include <map>
#include <string>
#include <functional>
#include <memory>

#include "standardformatdetector.h"

#include "formatdetector.h"

class Module;
class InterpreterConstructor;
/*!
 * @brief Manage the loading of modules
 *
 * When a \link Module module\endlink is \link addModule added\endlink the \link ModuleLoader module loader\endlink register the format detection
 * methods for the \link Module module\endlink (see Module::addFormatDetection). Then when a file needs to be parsed the \link ModuleLoader
 * module loader\link uses its \link StandardFormatDetector format detector\endlink to choose which \link Module module\endlink to assign.
 *
 * When a \link Module module\endlink is needed, the \link ModuleLoader module loader\endlink ask for requested \link Module modules to import\endlink
 * (see Module::requestImportations), load them, import them to the \link Module module\endlink and then loads the \link Module module\endlink itself
 * (see Module::doLoad).
 */
class ModuleLoader
{
public:
    ModuleLoader();

    /**
     * @brief Add a module and register its \link Module::addFormatDetection format detection methods\endlink
     *
     * @param key unique identifier for the module, used to \link loadModule load a module explicitly\endlink
     * or \link Module::requestImportations request importations\endlink.
     *
     * @param module A pointer to the module is given and the \link ModuleLoader\endlink takes ownership of the module,
     * the module is then destroyed when the \link Moduleloader module loader\endlink itself is destroyed.
     */
    void addModule(const std::string& key, Module* module);

    /**
     * @brief Generate and add \link FromFileModule HMScript modules\endlink from a folder
     *
     * The key for the module are the name of the files (extension excluded)
     *
     * The files are compiled again only if the compiled file is less recent than the original file
     */
    void addFolder(const std::string& folderName, const InterpreterConstructor& interpreterConstructor);

    /**
     * @brief Give constant access to a module and loads it has not yet been loaded
     */
    const Module& getModule(const std::string& key) const;

    /**
     * @brief Give constant access to a module able to handle the file and loads it has not yet been loaded
     *
     * the \link ModuleLoader module loader\endlink uses its \link StandardFormatDetector format detector\endlink
     * to choose which \link Module module\endlink to assign
     */
    const Module& getModule(File &file) const;

private:
    std::map<std::string, std::shared_ptr<Module> > modules;

    StandardFormatDetector formatDetector;

    //Non-copyable
    ModuleLoader(const ModuleLoader&) = delete;
    ModuleLoader& operator=(const ModuleLoader&) = delete;
};

#endif // MODULELOADER_H
