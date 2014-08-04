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

#ifndef STANDARDFORMATDETECTOR_H
#define STANDARDFORMATDETECTOR_H

#include "core/formatdetector/compositeformatdetector.h"
#include "core/formatdetector/magicformatdetector.h"
#include "core/formatdetector/syncbyteformatdetector.h"
#include "core/formatdetector/extensionformatdetector.h"

/*!
 * @brief Practical \link FormatDetector format detection\endlink implementation used by
 * the \link ModuleLoader module loader\endlink.
 *
 * The class is a \link CompositeFormatDetector Composite detector\endlink composed of a
 * \link MagicFormatDetector magic number detector\endlink, a
 * \link SyncbyteFormatDetector syncbyte detector\endlink  and a
 * \link ExtensionFormatDetector extension detector\endlink.
 *
 * The class can generate adders that give an interface for the \link Module modules\endlink
 * to add detection methods.
 */
class StandardFormatDetector : public CompositeFormatDetector
{
public:
    /**
     * @brief Interface for the \link Module modules\endlink
     * to add detection methods.
     */
    class Adder
    {
    public:
        /** @brief See MagicFormatDetector**/
        void addMagicNumber(const std::string& magicNumber);
        /** @brief See MagicFormatDetector**/
        void addMagicNumber(const std::string& magicNumber, int offset);
        /** @brief See SyncbyteFormatDetector**/
        void addSyncbyte(uint8_t syncbyte, int packetlength);
        /** @brief See ExtensionFormatDetector**/
        void addExtension(const std::string& extension);
    private:
        friend class StandardFormatDetector;
        Adder(StandardFormatDetector& detector, const std::string& format);
        StandardFormatDetector& detector;
        const std::string& format;
    };


    StandardFormatDetector();

    /**
     * @brief Generate a new adder with the given format
     */
    Adder* newAdder(const std::string& format);


private:
    ExtensionFormatDetector _extensionDetector;
    MagicFormatDetector     _magicDetector;
    SyncbyteFormatDetector  _syncbyteDetector;
};

#endif // STANDARDFORMATDETECTOR_H
