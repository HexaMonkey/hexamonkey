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

#ifndef STRUCT_H
#define STRUCT_H

#include <stdint.h>

typedef union _elem
{
	void* node;
	int64_t  i;
	uint64_t u;
	double   f;
	char*    s;
} elem;

typedef struct _ast
{
	uint32_t id;
	uint64_t size;
	int32_t line_number;
	void* parent;
	elem first_child;
	void* next_sibling;
} ast;

typedef union _content
{
	int64_t  i;
	uint64_t u;
	double   f;
	char*    s;
} content;

typedef struct _stack
{
	int32_t id;
	int32_t count;
	content content;
	void* st;
} stack;

typedef struct _int_stack
{
	int i;
	void* st;
} int_stack;

#endif
