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

#ifndef MASKUTIL_H
#define MASKUTIL_H

#include <stdint.h>

/*!
 * @brief Get the index of the most signifigant bit
 */
int msb(uint8_t byte);

/*!
 * @brief Get a bit mask with the n least siginifigant bits set. Behaviour for
 * i > 8 or i < 0 is unspecified.
 */
uint8_t lsbMask(int n);

/**
 * @brief Get the number of bits in a byte
 */
uint8_t popCount(uint64_t word);

#endif // MASKUTIL_H
