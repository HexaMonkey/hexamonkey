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

#include "compositeformatdetector.h"
#include "magicformatdetector.h"
#include "syncbyteformatdetector.h"
#include "extensionformatdetector.h"


class StandardFormatDetector : public CompositeFormatDetector
{
public:
    class Adder
    {
    public:
        void addMagicNumber(const std::string& magicNumber);
        void addMagicNumber(const std::string& magicNumber, int offset);
        void addSyncbyte(uint8_t syncbyte, int packetlength);
        void addExtension(const std::string& extension);
    private:
        friend class StandardFormatDetector;
        Adder(StandardFormatDetector& detector, const std::string& format);
        StandardFormatDetector& detector;
        const std::string& format;
    };


    StandardFormatDetector();
    ExtensionFormatDetector& extensionDetector();
    MagicFormatDetector&     magicDetector();
    SyncbyteFormatDetector&  syncbyteDetector();

    Adder* newAdder(const std::string& key);


private:
    ExtensionFormatDetector _extensionDetector;
    MagicFormatDetector     _magicDetector;
    SyncbyteFormatDetector  _syncbyteDetector;
};

#endif // STANDARDFORMATDETECTOR_H
