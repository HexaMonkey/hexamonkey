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

#ifndef WRITE_H
#define WRITE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h> 
#include <stdint.h> 

int ebml_int_size(uint64_t value)
{
    value++;
    int size = 1;
	uint64_t max_value;
    for(max_value = 1<<7; value >= max_value; max_value<<=7)
    {
        size ++;
		if(size == 8)
			break;
    }
    return size;
}

int int_size(int64_t value)
{
	int size = 1;
	int64_t max_value = 1<<7;
    for(max_value = 1<<7; value >= max_value || value < -max_value; max_value<<=8)
    {
        size ++;
		if(size == 8)
			break;
    }
    return size;
}

int uint_size(uint64_t value)
{
	int size = 1;
	uint64_t max_value = 1<<8;
    for(max_value = 1<<8; value >= max_value; max_value<<=8)
    {
        size ++;
		if(size == 8)
			break;
    }
    return size;
}

void write_ebml_int(FILE* file, uint64_t i)
{
	int size = ebml_int_size(i);
	unsigned char* p_first = (char*)&i;
	unsigned char* p_current  = p_first + (size - 1);
	fputc(*p_current | 1<<(8-size), file);
	for(p_current--; p_current>=p_first; p_current--)
	{
		fputc(*p_current, file);
	}
}

void write_int(FILE* file, int64_t i, int size)
{
	unsigned char* p_first = (char*)&i;
	unsigned char* p_current;
	for(p_current = p_first + (size - 1); p_current>=p_first; p_current--)
	{
		fputc(*p_current, file);
	}
}

void write_uint(FILE* file, uint64_t u, int size)
{
	unsigned char* p_first = (char*)&u;
	unsigned char* p_current;
	for(p_current = p_first + (size - 1); p_current>=p_first; p_current--)
	{
		fputc(*p_current, file);
	}
}

void write_string(FILE* file, char* s, int size)
{
	int count;
	for(count = 0; count < size; count ++)
		fputc(s[count],file);
}

void write_float(FILE* file, double f)
{
	unsigned char* p_first = (char*)&f;
	unsigned char* p_current;
	for(p_current = p_first + 7; p_current>=p_first; p_current--)
	{
		fputc(*p_current, file);
	}
}

#endif
