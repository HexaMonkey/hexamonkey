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

#include "core/formatdetector/standardformatdetector.h"

StandardFormatDetector::StandardFormatDetector():
    _syncbyteDetector(64)
{
    addDetector(_magicDetector);
    addDetector(_syncbyteDetector);
    addDetector(_extensionDetector);
}

StandardFormatDetector::Adder *StandardFormatDetector::newAdder(const std::string &format)
{
    return new Adder(*this, format);
}


void StandardFormatDetector::Adder::addMagicNumber(const std::string &magicNumber)
{
    detector._magicDetector.addMagicNumber(format, magicNumber);
}

void StandardFormatDetector::Adder::addMagicNumber(const std::string &magicNumber, int offset)
{
    detector._magicDetector.addMagicNumber(format, magicNumber, offset);
}

void StandardFormatDetector::Adder::addSyncbyte(uint8_t syncbyte, int packetlength)
{
    detector._syncbyteDetector.addSyncbyte(format, syncbyte, packetlength);
}

void StandardFormatDetector::Adder::addExtension(const std::string &extension)
{
    detector._extensionDetector.addExtension(format, extension);
}

StandardFormatDetector::Adder::Adder(StandardFormatDetector &detector, const std::string &format)
    :detector(detector), format(format)
{
}
