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

#ifndef MAGICFORMATDETECTOR_H
#define MAGICFORMATDETECTOR_H

#include "formatdetector.h"

/*!
 * @brief \link FormatDetector Format detection\endlink implementation  that
 * uses magic number (or file signature).
 *
 * A magic number is a byte string, found in the beginning of the file and unique to
 * a format. A list can be found here : http://www.garykessler.net/library/file_sigs.html
 *
 * When two magic numbers match a \link File file\endlink the longest will have priority
 */
class MagicFormatDetector : public FormatDetector
{
public:
    /**
     * @brief Map a magic number matching the beginning of the file to a format
     *
     * @param magicNumber the magic number shall be given as a string with space
     * separated bytes given as two-digit hex numbers. "xx" can be given instead
     * of a number to specify that there is no requirement for the byte.
     */
    void addMagicNumber(const std::string &format, const std::string& magicNumber);
    /**
     * @brief Shift a magic number and map it to a format.
     *
     * Equivalent to addMagicNumber(format, "xx "+magicNumber, offset-1) if offset > 0
     */
    void addMagicNumber(const std::string &format, const std::string& magicNumber, int offset);

private:
    virtual std::string doGetFormat(File& file) const override;
    std::map<std::string, std::string> _magicNumbers;
};

#endif // MAGICFORMATDETECTOR_H
