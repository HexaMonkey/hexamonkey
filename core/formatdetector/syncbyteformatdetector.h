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

#include "core/file/file.h"
#include "core/formatdetector/formatdetector.h"

/*!
 * @brief \link FormatDetector Format detection\endlink implementation that
 * looks for the periodic repetition of a byte.
 *
 * This is typically used to detect a format using packets of fixed length each
 * beginning with a fixed byte called syncbyte.
 */
class SyncbyteFormatDetector : public FormatDetector
{
public:
    /**
     * @brief Construct a \link SyncbyteFormatDetector syncbyte detector\endlink
     * and specifiy the number of periods (or packets) that needs to be checked
     * to sucessfully detect a format.
     */
    SyncbyteFormatDetector(int numberOfPeriods);

    /**
     * @brief Map a syncbyte to a format
     */
    void addSyncbyte(const std::string &format, uint8_t syncbyte, int packetlength);
protected:
    virtual std::string doGetFormat(File& file) const override;
private:
    std::map<std::string, std::pair<uint8_t, int> > _formats;

    const int numberOfPeriods;
};

#endif // SYNCBYTEFORMATDETECTOR_H
