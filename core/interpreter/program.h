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

#ifndef EBMLOBJECT_H
#define EBMLOBJECT_H

#include "object.h"

class Program
{
public:
    template<class It>
    class _const_iterator : public std::iterator<std::input_iterator_tag, Program>
    {
        friend class Program;
        It _it;

        _const_iterator<It>(const It& it):_it(it){}
        public:
            _const_iterator<It>(){}
            _const_iterator<It>& operator++() {++_it; return *this;}
            _const_iterator<It> operator++(int) {const_iterator dup(*this); ++_it; return dup;}
            _const_iterator<It>& operator--() {--_it; return *this;}
            _const_iterator<It> operator--(int) {const_iterator dup(*this); --_it; return dup;}
            Program operator*() const {return Program(**_it);}
            bool operator==(const _const_iterator<It>& other) const {return _it==other._it;}
            bool operator!=(const _const_iterator<It>& other) const {return !(*this==other);}
    };

    typedef _const_iterator< Object::iterator > const_iterator;
    typedef _const_iterator< Object::reverse_iterator > const_reverse_iterator;

    Program();
    Program(Object& object);

    uint32_t id() const;
    const Variant& payload() const;
    int size() const;
    Program elem(int index) const;

    const_iterator begin() const;
    const_iterator end() const;
    const_reverse_iterator rbegin() const;
    const_reverse_iterator rend() const;

private:
    Object* _object;
};

#endif // EBMLOBJECT_H
