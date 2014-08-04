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

#ifndef SHOWCASE_H
#define SHOWCASE_H

#include <list>
#include <string>

/*!
 * @brief List of names for children of \link Object objects\endlink
 * that should be displayed prominently.
 */
class Showcase
{
public:
    typedef std::list<std::string> container;
    typedef container::iterator iterator;
    typedef container::const_iterator const_iterator;

    Showcase();

    void add(const std::string& elem);

    const_iterator begin() const;
    const_iterator end() const;

    bool empty() const;
    int size() const;
private:
    container elems;

};

#endif // SHOWCASE_H
