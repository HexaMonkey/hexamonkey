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

#ifndef COMPOSITEFORMATDETECTOR_H
#define COMPOSITEFORMATDETECTOR_H

#include "core/formatdetector/formatdetector.h"

/*!
 * @brief \link FormatDetector Format detection\endlink implementation that
 * uses several other \link FormatDetector detectors\endlink.
 *
 * The \link FormatDetector detectors\endlink are used on the file one after the
 * other until one succeed.
 */
class CompositeFormatDetector : public FormatDetector
{
public:
    /**
     * @brief Put a \link FormatDetector detectors\endlink in the end of the detection list.
     *
     * The \link FormatDetector detectors\endlink added will be used last and therefore will
     * have the lowest priority.
     */
    void addDetector(FormatDetector& detector);
private:
    std::string doGetFormat(File& file) const override;

    std::vector<FormatDetector*> _detectors;
};

#endif // COMPOSITEFORMATDETECTOR_H
