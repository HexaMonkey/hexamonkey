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

#include <stdio.h>
#include <string.h>

#include "write.h"
#include "model.h"

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
	void* parent;
	elem first_child;
	void* next_sibling;
} ast;

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

ast root = {ROOT_ID, 0, NULL, NULL, NULL};
ast* current_node = &root;
int line_number=1;
int indentation = 0;

void print_indentation()
{
	int i;
	for(i = 0; i < indentation; ++i)
	{
		printf("\t");
	}
}

ast* open_node(uint32_t id)
{
	current_node = append_node(current_node, id);
#ifdef VERBOSE	
	print_indentation();
	printf("<%s>\n",hmcElemNames[id]);
#endif
	++indentation;
}

ast* insert_first_node(uint32_t id)
{
	current_node = prepend_node(current_node, id);	
#ifdef VERBOSE	
	print_indentation();
	printf("<%s>\n",hmcElemNames[id]);
#endif
	++indentation;
}

ast* close_node()
{
	--indentation;
#ifdef VERBOSE	
	print_indentation();
	printf("</%s>\n",hmcElemNames[current_node->id]);
#endif
	
	ast* parent = current_node->parent;
	
	parent->size += ebml_int_size(current_node->id);
	parent->size += ebml_int_size(current_node->size);
	parent->size += current_node->size;
	current_node = parent;
}

void set_integer(int64_t i)
{
#ifdef VERBOSE
	print_indentation();
	printf("%d\n",i);
#endif
	current_node->size = int_size(i);
	current_node->first_child.i = i;
}

void set_uinteger(uint64_t u)
{
#ifdef VERBOSE	
	print_indentation();
	printf("%u\n",u);
#endif
	current_node->size = uint_size(u);
	current_node->first_child.u = u;
}

void set_string(char* s)
{
#ifdef VERBOSE	
	print_indentation();
	printf("%s\n",s);
#endif
	current_node->size = strlen(s);
	current_node->first_child.s = s;
}

void set_float(double f)
{
#ifdef VERBOSE	
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
	switch(hmcElemTypes[node->id])
	{
		case MASTER:
			for(child = node->first_child.node; child != NULL; child = child->next_sibling)
			{
				write_node(file, child);
			}
			break;
			
		case INTEGER:
			write_int(file, node->first_child.i, node->size);
			break;
			
		case UINTEGER:
			write_uint(file, node->first_child.u, node->size);
			break;
			
		case STRING:
			write_string(file, node->first_child.s, node->size);
			break;
			
		case FLOAT:
			write_float(file, node->first_child.f);
			break;
	}
}

typedef union _content
{
	int64_t  i;
	uint64_t u;
	double   f;
	char*    s;
} content;

typedef struct _stack
{
	uint32_t id;
	elem content;
	void* st;
} stack;

typedef struct _int_stack
{
	int i;
	void* st;
} int_stack;

stack* current_stack = NULL;

void new_stack(uint32_t id)
{
	stack* s = malloc(sizeof(stack));
	s->id = id;
	s->st = current_stack;
	current_stack = s;
}

void push_master(uint32_t id, int count)
{
	new_stack(id);
	current_stack->content.i = count;
}

void push_integer(uint32_t id, int64_t i)
{
	new_stack(id);
	current_stack->content.i = i;
}

void push_uinteger(uint32_t id, uint64_t u)
{
	new_stack(id);
	current_stack->content.u = u;
}

void push_float(uint32_t id, double f)
{
	new_stack(id);
	current_stack->content.f = f;
}

void push_string(uint32_t id, char* s)
{
	new_stack(id);
	current_stack->content.s = s;
}

void pop()
{
	stack* popped = current_stack;
	current_stack = popped->st;
	
	switch(hmcElemTypes[popped->id])
	{
		case MASTER:
			if(hmcElemAssoc[popped->id] && current_node->id == popped->id)
			{
				int i;
				for(i=0; i<popped->content.i;++i) 
					pop();
			}
			else
			{
				insert_first_node(popped->id);
				int i;
				for(i=0; i<popped->content.i;++i)
					pop();
				close_node();
			}
			break;
			
		case INTEGER:
			insert_first_node(popped->id);
			set_integer(popped->content.i);
			close_node();
			break;
			
		case UINTEGER:
			insert_first_node(popped->id);
			set_uinteger(popped->content.u);
			close_node();
			break;
			
		case STRING:
			insert_first_node(popped->id);
			set_string(popped->content.s);
			close_node();
			break;
			
		case FLOAT:
			insert_first_node(popped->id);
			set_float(popped->content.f);
			close_node();
			break;
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
	if(hmcElemTypes[s->id] == MASTER)
	{
		int i;
		
		for(i=0; i<s->content.i;++i)
		{
			result += _stash(number);
		}
	}
	return result; 
}

void unstash(int number)
{
	int i = 0;
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
		new_stack(current_stash->id);
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

void add_operator(int id)
{
	insert_first_node(OPERATOR);
	set_integer(id);
	close_node();
}

void handle_binary_op(int id)
{
	stash(0);stash(0);
	push_integer(OPERATOR, id);
	unstash(0);unstash(0);
}

void handle_unary_op(int id)
{
	stash(0);
	push_integer(OPERATOR, id);
	unstash(0);
}

void handle_ternary_op(int id)
{
	stash(0);stash(0);stash(0);
	push_integer(OPERATOR, id);
	unstash(0);unstash(0);unstash(0);
}