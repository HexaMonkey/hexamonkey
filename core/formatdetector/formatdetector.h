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

#ifndef FORMATDETECTOR_H
#define FORMATDETECTOR_H

#include <string>
#include <vector>
#include <map>

#include "core/file/file.h"

/*!
 * @brief Class used to identify the format of a \link File file\endlink.
 *
 * The format is a string, which is used by the \link ModuleLoader module loader\endlink
 * to assign a \link Module module\endlink to a \link File file\endlink.
 *
 * The class must be subclassed to implement a detection method.
 */
class FormatDetector
{
public:
    /**
     * @brief Identify the format of the file
     *
     * Return an empty string if the format hasn't been detected.
     */
    std::string getFormat(File& file) const;
protected:
    virtual std::string doGetFormat(File& file) const = 0;
};




#endif // FORMATDETECTOR_H
