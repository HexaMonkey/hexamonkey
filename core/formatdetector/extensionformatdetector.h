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

#ifndef EXTENSIONFORMATDETECTOR_H
#define EXTENSIONFORMATDETECTOR_H

#include "formatdetector.h"

/*!
 * @brief The ExtensionFormatDetector class
 */
class ExtensionFormatDetector : public FormatDetector
{
public:
    void addExtension(const std::string& format, const std::string& extension);
protected:
    virtual std::string doGetFormat(File& file) const override;
private:
    std::map<std::string, std::string> _extensions;
};

#endif // EXTENSIONFORMATDETECTOR_H
