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

#ifndef ITERUTIL_H
#define ITERUTIL_H

#include <utility>

/**
 * Iteration wrapper to
 */
template<class Cont>
class const_reverse_wrapper
{
  const Cont& container;

public:
  const_reverse_wrapper(const Cont& cont) : container(cont){ }
  decltype(container.rbegin()) begin() const { return container.rbegin(); }
  decltype(container.rend()) end() const { return container.rend(); }
};

template<class Cont>
class reverse_wrapper
{
  Cont& container;

public:
  reverse_wrapper(Cont& cont) : container(cont){ }
  decltype(container.rbegin()) begin() { return container.rbegin(); }
  decltype(container.rend()) end() { return container.rend(); }
};

template<class Cont>
const_reverse_wrapper<Cont> reverse(const Cont& cont)
{
  return const_reverse_wrapper<Cont>(cont);
}

template<class Cont>
reverse_wrapper<Cont> reverse(Cont& cont)
{
  return reverse_wrapper<Cont>(cont);
}

template<class iterator>
class range
{
  iterator _begin;
  iterator _end;

public:
  range(const iterator& begin, const iterator& end) : _begin(begin), _end(end) {}
  range(const std::pair<iterator, iterator>& pair) : _begin(pair.first), _end(pair.second) {}

  iterator begin() {return _begin;}
  iterator end()   {return _end;}
};

#endif // ITERUTIL_H
