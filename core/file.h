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

#ifndef FILE_H
#define FILE_H

#include <iostream>
#include <fstream>
#include <stdint.h>


/** @brief High-level input stream operations on files with bit precision

The class is implemented as an adaptor for a std::ifstream instance that
reimplements common operation with bit precision instead of byte precision*/
class File
{
public:
    File();

    /** @brief Sets the path to the file*/
    void setPath(const std::string& path);

    /** @brief Returns the path to the file*/
    const std::string& path() const;

    /** @brief Opens stream with the given path*/
    void open();

    /** @brief Closes stream*/
    void close();

    /** @brief Clears the file error flags*/
    void clear();


    /** @brief Extracts bits from stream

    Puts the result in a byte array already allocated
    the result is right aligned and zero padded*/
    void read(char* s, int64_t size);

    /** @brief Offsets the position

     * \param off Offset to apply in bits.
     * \param dir Where to start from to apply the offset.
     * begin (std::ios_base::beg), current (std::ios_base::cur) or
     * end (std::ios_base::end).
     */
    void seekg(int64_t off, std::ios_base::seekdir dir);

    /** @brief Returns the current stream position */
    int64_t tellg();

    /** @brief Returns the size of the file*/
    int64_t size();

    /** @brief Checks if data can be recovered from the stream*/
    bool good();


private:
    File& operator=(const File&) = delete;
    File(const File&) = delete;

    std::string _path;
    std::ifstream _file;

    char _bitPosition;
};

/**
 * @brief RAII object that insure that the \link File file\endlink returns to its
 * original position when the ressource is liberated
 */
class FileAnchor
{
public:
    FileAnchor(File& file);
    ~FileAnchor();
private:
    File& file;
    int64_t position;
};

#endif // FILE_H
