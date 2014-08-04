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

#ifndef CSVREADER_H
#define CSVREADER_H

#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Parse a cvs file
 */
class CSVReader {
public:
    /**
     * @brief Construct a reader from an input stream
     */
    CSVReader(std::istream &input);

    /**
     * @brief Check if the input stream still has a line
     */
    bool hasNextLine();

    /**
     * @brief Parse a line into a string vector
     */
    void readNextLine(std::vector<std::string> &fields);

    /**
     * @brief Ignore the next line in the input stream
     */
    void ignoreNextLine();
private:
    std::istream &_input;
};

#endif // CSVREADER_H
