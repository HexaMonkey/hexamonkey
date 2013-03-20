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

#ifndef SYNCBYTEFORMATDETECTOR_H
#define SYNCBYTEFORMATDETECTOR_H

#include "formatdetector.h"

/*!
 * \brief The SyncbyteFormatDetector class
 */
class SyncbyteFormatDetector : public FormatDetector
{
public:
    void addSyncbyte(const std::string &format, uint8_t syncbyte, int packetlength);
protected:
    virtual std::string doGetFormat(File& file) const override;
private:
    std::map<std::string, std::pair<uint8_t, int> > _formats;

    static const int check = 64;
};

#endif // SYNCBYTEFORMATDETECTOR_H
