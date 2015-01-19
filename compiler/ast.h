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

#ifndef AST_H
#define AST_H

#include <stdio.h>
#include <string.h>

#include "write.h"
#include "model.h"
#include "struct.h"

#ifdef POP_DEBUG
#define VERBOSE
#endif

#ifdef WRITE_DEBUG
#define VERBOSE
#endif

ast* last_sibling(ast* node)
{
	if(node->next_sibling == NULL)
		return node;
	else
		return last_sibling((ast*)node->next_sibling);
}

ast* new_node(ast* parent, uint32_t id)
{
	ast* node = malloc(sizeof(ast));
	
	node->id = id;
	node->size = 0;
	node->line_number = -1;
	node->parent = parent;
	node->first_child.node = NULL;
	node->next_sibling = NULL;
	
	return node;
}

ast* append_node(ast* parent, uint32_t id)
{
	ast* node = new_node(parent, id);
	
	if(parent->first_child.node == NULL)
		parent->first_child.node = node;
	else	
		last_sibling(parent->first_child.node)->next_sibling = node;

	return node;
}

ast* prepend_node(ast* parent, uint32_t id)
{
	ast* node = new_node(parent, id);
	
	node->next_sibling = parent->first_child.node;
	parent->first_child.node = node;

	return node;
}

ast root = {ROOT_ID, 0, -1, NULL, NULL, NULL};
ast* current_node = &root;
int line_number=1;

#ifdef VERBOSE
int indentation = 0;
void print_indentation()
{
	int i;
	for(i = 0; i < indentation; ++i)
	{
		printf(" ");
	}
}
#endif

ast* insert_first_node(uint32_t id)
{
	current_node = prepend_node(current_node, id);	
#ifdef POP_DEBUG	
	print_indentation();
	printf("<%s>\n",hmcElemNames[id]);
	++indentation;
#endif
}

ast* close_node()
{
#ifdef POP_DEBUG
	--indentation;	
	print_indentation();
	printf("</%s>\n",hmcElemNames[current_node->id]);
#endif
	
	ast* parent = current_node->parent;
	
	if(parent != NULL) 
	{
		parent->size += ebml_int_size(current_node->id)
					 +  ebml_int_size(current_node->size)
	                 +  current_node->size;
		
		if(current_node->line_number != -1) 
		{
			if(parent->line_number == -1 || parent->line_number > current_node->line_number)
			{
				parent->line_number = current_node->line_number;
			}
		}
	}
	
	current_node = parent;	
}

void set_integer(int64_t i)
{
#ifdef POP_DEBUG
	print_indentation();
	printf("%d\n",i);
#endif
	current_node->size = int_size(i);
	current_node->first_child.i = i;
}

void set_uinteger(uint64_t u)
{
#ifdef POP_DEBUG	
	print_indentation();
	printf("%u\n",u);
#endif
	current_node->size = uint_size(u);
	current_node->first_child.u = u;
}

void set_string(char* s)
{
#ifdef POP_DEBUG	
	print_indentation();
	printf("%s\n",s);
#endif
	current_node->size = strlen(s);
	current_node->first_child.s = s;
}

void set_float(double f)
{
#ifdef POP_DEBUG	
	print_indentation();
	printf("%f\n",f);
#endif
	current_node->size = 8;
	current_node->first_child.f = f;
}

void write_node(FILE* file, ast* node)
{
	write_ebml_int(file, node->id);
	write_ebml_int(file, node->size);
	ast* child;
#ifdef WRITE_DEBUG
	print_indentation();
	printf("<%s line='%d'>",hmcElemNames[node->id], node->line_number);
#endif
	switch(hmcElemTypes[node->id])
	{
		case MASTER:
#ifdef WRITE_DEBUG
			++indentation;
			printf("\n");
#endif
			for(child = node->first_child.node; child != NULL; child = child->next_sibling)
			{
				write_node(file, child);
			}
#ifdef WRITE_DEBUG
			--indentation;
			print_indentation();
#endif
			break;
			
		case INTEGER:
#ifdef WRITE_DEBUG
			printf("%d", node->first_child.i);
#endif
			write_int(file, node->first_child.i, node->size);
			break;
			
		case UINTEGER:
#ifdef WRITE_DEBUG
			printf("%d", node->first_child.u);
#endif
			write_uint(file, node->first_child.u, node->size);
			break;
			
		case STRING:
#ifdef WRITE_DEBUG
			printf("%s", node->first_child.s);
#endif
			write_string(file, node->first_child.s, node->size);
			break;
			
		case FLOAT:
#ifdef WRITE_DEBUG
			printf("%f", node->first_child.f);
#endif
			write_float(file, node->first_child.f);
			break;
	}
#ifdef WRITE_DEBUG
	printf("</%s>\n",hmcElemNames[node->id]);
#endif
}

stack* current_stack = NULL;

void push_line()
{
	stack* s = malloc(sizeof(stack));
	s->id = -1;
	s->count = 0;
	s->content.u = line_number;
	s->st = current_stack;
	
	current_stack = s;
}

void new_stack(int32_t id, int32_t count)
{
	if(count == 0) 
	{
		push_line();
		count = 1;
	}

	stack* s = malloc(sizeof(stack));
	s->id = id;
	s->count = count;
	s->st = current_stack;
	current_stack = s;
}

void push_master(int32_t id, int32_t count)
{
	new_stack(id, count);
}

void push_integer(int32_t id, int64_t i)
{
	new_stack(id, 0);
	current_stack->content.i = i;
}

void push_uinteger(int32_t id, uint64_t u)
{
	new_stack(id, 0);
	current_stack->content.u = u;
}

void push_float(int32_t id, double f)
{
	new_stack(id, 0);
	current_stack->content.f = f;
}

void push_string(int32_t id, char* s)
{
	new_stack(id, 0);
	current_stack->content.s = s;
}

void ignore()
{
	stack* popped = current_stack;
	current_stack = popped->st;
	
	int i;
	for(i=0; i<popped->count;++i)
		ignore();

	free(popped);
}

void pop()
{
	stack* popped = current_stack;
	current_stack = popped->st;

	int should_insert =  (popped->id != -1) //not line
				   && !(hmcElemTypes[popped->id] == MASTER && hmcElemAssoc[popped->id] && current_node->id == popped->id);//not associative

	if(should_insert) 
	{
		insert_first_node(popped->id);
	}

	if(popped->id == -1) 
	{
		if(current_node != NULL && (current_node->line_number == -1 || current_node->line_number > popped->content.u))
		{
			current_node->line_number = popped->content.u;
		}
	}
	else
	{
		switch(hmcElemTypes[popped->id])
		{
			case INTEGER:
				set_integer(popped->content.i);
				break;

			case UINTEGER:
				set_uinteger(popped->content.u);
				break;

			case STRING:
				set_string(popped->content.s);
				break;

			case FLOAT:
				set_float(popped->content.f);
				break;
		}
	}

	int i;
	for(i=0; i<popped->count;++i)
		pop();


	if(should_insert) 
	{
		close_node();
	}

	free(popped);
}

int empty()
{
	return current_stack == NULL;
}


stack* current_stashes[] = {NULL, NULL}; 
int_stack* stashes_counts[] = {NULL, NULL};

void stash(int number)
{
	int_stack* s = malloc(sizeof(int_stack));
	s->i = _stash(number);
	s->st = stashes_counts[number];
	stashes_counts[number] = s;
}

int _stash(int number)
{
	stack* s = current_stack;
	current_stack = s->st;
	s->st = current_stashes[number];
	current_stashes[number] = s;

	int result = 1;
	int i;
	for(i = 0; i < s->count; ++i)
	{
		result += _stash(number);
	}

	return result; 
}

void unstash(int number)
{
	int i;
	for(i = 0; i < stashes_counts[number]->i; ++i)
	{
		stack* s = current_stashes[number];
		current_stashes[number] = s->st;
		s->st = current_stack;
		current_stack = s;
	}
	int_stack* old = stashes_counts[number];
	stashes_counts[number] = old->st;
	free(old);
}

void copy_stashed(int number)
{
	int i = 0;
	stack* current_stash = current_stashes[number];
	int_stack* current_count = stashes_counts[number];
	for(i = 0; i < current_count->i; ++i)
	{
		new_stack(current_stash->id, current_stash->count);
		current_stack->content = current_stash->content;

		current_stash = current_stash->st;
	}
}

void del_stashed(int number)
{
	int i;
	for(i = 0; i < stashes_counts[number]->i; ++i)
	{
		stack* popped = current_stashes[number];
		current_stashes[number] = popped->st;
		free(popped);
	}
	int_stack* old = stashes_counts[number];
	stashes_counts[number] = old->st;
	free(old);
}

void handle_op(int id)
{
	int parameterCount = operatorParameterCount[id];
	
	int i;
	for(i = 0; i < parameterCount; ++i)
	{
		stash(0);
	}
	
	push_integer(OPERATOR, id);
	
	for(i = 0; i < parameterCount; ++i)
	{
		unstash(0);
	}
	
	push_master(RIGHT_VALUE, parameterCount+1);
}

#ifdef STACK_DEBUG
void dump_stack()
{
	stack* st = current_stack;
	while(st) {
		if(st->id == -1) 
		{
			printf("0 - line %d\n", st->content.u);
		}
		else
		{
			printf("%d - %s\n", st->count, hmcElemNames[st->id]);
		}
		st = st->st;
	}
}
#endif

#endif
