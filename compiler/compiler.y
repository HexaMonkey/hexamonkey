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

%error-verbose

%{ 
    #include "ast.h"  
    #include "header.h"
    extern FILE *yyin;   
    void yyerror (char const *);
    int yydebug=1;                                                                    
    int indent=0; 
    int infos=0;
%}    

%union {
    long long          i;
    unsigned long long u;
    char              *s;
    double             f;
}

%token CLASS_TOKEN EXTENDS_TOKEN AS_TOKEN TYPEDEF_TOKEN FOR_TOKEN WHILE_TOKEN DO_TOKEN RETURN_TOKEN BREAK_TOKEN CONTINUE_TOKEN VAR_TOKEN FORWARD_TOKEN TO_TOKEN FUNCTION_TOKEN CONST_TOKEN ELLIPSIS_TOKEN HEADER_TOKEN
%right IF_TOKEN ELSE_TOKEN

%token IMPORT_TOKEN ADD_MAGIC_NUMBER_TOKEN ADD_EXTENSION_TOKEN ADD_SYNCBYTE_TOKEN SHOWCASED_TOKEN REMOVE_TOKEN

%token STRUCT_TOKEN

%left ',' 
%right SUBSCOPE_ASSIGN_TOKEN
%right '=' RIGHT_ASSIGN_TOKEN LEFT_ASSIGN_TOKEN ADD_ASSIGN_TOKEN SUB_ASSIGN_TOKEN MUL_ASSIGN_TOKEN DIV_ASSIGN_TOKEN MOD_ASSIGN_TOKEN AND_ASSIGN_TOKEN XOR_ASSIGN_TOKEN OR_ASSIGN_TOKEN
%nonassoc '?' ':'
%left OR_TOKEN  
%left AND_TOKEN
%left '|'
%left '^'
%left '&'
%nonassoc EQ_TOKEN NE_TOKEN 
%nonassoc '>' GE_TOKEN
%nonassoc '<' LE_TOKEN
%left RIGHT_TOKEN LEFT_TOKEN
%left '+' '-'
%left '*' '/' '%'
%right NOT_TOKEN
%right BITWISE_NOT_TOKEN
%right OPP
%right INC_TOKEN DEC_TOKEN
%left SUF_INC SUF_DEC
%right DEREF

%token <s> IDENT
%token <s> A_IDENT
%token <s> P_IDENT
%token <s> MAGIC_NUMBER

%token NULL_TOKEN
%token UNDEFINED_TOKEN
%token EMPTY_STRING_TOKEN
%token <s> STRING_VALUE 
%token <i> INT_VALUE 
%token <u> UINT_VALUE  
%token <f> FLOAT_VALUE
 
%% /* Grammar rules and actions follow */
main:file
  
file: format_detection_additions imports class_declarations

format_detection_additions:
    /*empty*/ {push_master(FORMAT_DETECTION_ADDITIONS,0);}
   |format_detection_additions format_detection_addition {push_master(FORMAT_DETECTION_ADDITIONS,2);}
;

imports:
    /*empty*/ {push_master(IMPORTS,0);}
    |imports import_token import_list {push_master(IMPORTS, 3);}
;

import_token:
	IMPORT_TOKEN {push_line();}

import_list:
    identifier {push_master(IMPORTS,1);}
   |import_list identifier {push_master(IMPORTS,2);}

class_declarations:
    /*empty*/ {push_master(CLASS_DECLARATIONS,0);}
   |class_declarations class_declaration {push_master(CLASS_DECLARATIONS,2);}
   |class_declarations forward {push_master(CLASS_DECLARATIONS,2);}
   |class_declarations function_declaration {push_master(CLASS_DECLARATIONS,2);}
;
   
format_detection_addition:
    ADD_MAGIC_NUMBER_TOKEN {push_integer(OPERATOR, ADD_MAGIC_NUMBER_OP);}  magic_number {push_master(FORMAT_DETECTION_ADDITION,2);}
   |ADD_EXTENSION_TOKEN {push_integer(OPERATOR, ADD_EXTENSION_OP);}  identifier {push_master(FORMAT_DETECTION_ADDITION,2);}
   |ADD_SYNCBYTE_TOKEN {push_integer(OPERATOR, ADD_SYNCBYTE_OP);}  uint_constant uint_constant {push_master(FORMAT_DETECTION_ADDITION,3);}
;

magic_number:
    MAGIC_NUMBER {push_string(STRING_CONSTANT, $1);}
;

class_declaration:    
    class_token class_info class_definition {push_master(CLASS_DECLARATION, 3);}
   |class_token class_infos class_definition 
    {
        stash(1);
		
	    copy_stashed(1);
		push_master(CLASS_DECLARATION, 3);
		push_master(CLASS_DECLARATIONS, 2);
		unstash(0);
		--infos;
        while(infos>0)
        {
            copy_stashed(1);
            push_master(CLASS_DECLARATION, 2);
            push_master(CLASS_DECLARATIONS, 2);
            unstash(0);
            --infos;
        }
        unstash(1);
        push_master(CLASS_DECLARATION, 2);
    }
;

class_token:
	CLASS_TOKEN {push_line();}

class_info:
    type_template extension specification {push_master(CLASS_INFO, 3);}

class_infos:
    class_info',' class_info {stash(0);++infos;} 
   |class_infos ',' class_info {stash(0);++infos;}
 
forward:
    forward_token type to_token type {push_master(FORWARD, 4);}

forward_token:
	FORWARD_TOKEN {push_line();}

to_token:
	TO_TOKEN {push_line();}
	
type_template:
     identifier type_template_argument_list {push_master(TYPE_TEMPLATE, 2);}
;

type_template_argument_list:
    /*empty*/ {push_master(ARGUMENT_DECLARATIONS, 0);}
    |'(' ')' {push_master(ARGUMENT_DECLARATIONS, 0);}
    |'(' type_template_arguments ')'
;

type_template_arguments:
      identifier {push_master(ARGUMENT_DECLARATIONS, 1);}
    | type_template_arguments ',' identifier {push_master(ARGUMENT_DECLARATIONS, 2);}
;

function_declaration:
    function_token identifier function_arguments execution_block {push_master(FUNCTION_DECLARATION, 4);}
;

function_token:
	FUNCTION_TOKEN {push_line();}

function_arguments:
    '(' function_argument_list ')'
   |'(' ')' {push_master(FUNCTION_ARGUMENTS, 0);}
;

function_argument_list:
    function_argument {push_master(FUNCTION_ARGUMENTS, 1);}
   |function_argument_list function_argument {push_master(FUNCTION_ARGUMENTS, 2);}   
;

function_argument:
    modifiable identifier default_value {push_master(FUNCTION_ARGUMENT, 3);}
    
modifiable:
    /*empty*/ {push_integer(MODIFIABLE, 1);}
   |CONST_TOKEN {push_integer(MODIFIABLE, 0);}
   
default_value:
    /*empty*/ {push_integer(NULL_CONSTANT, 0); push_master(RIGHT_VALUE, 1);}
   |'=' right_value

type_access:
    type {push_master(RIGHT_VALUE,1);}
   |'(' right_value ')'
;

type:
    identifier {push_master(ARGUMENTS,0); push_master(TYPE, 2);} 
   |explicit_type
   
explicit_type:
    identifier right_value_arguments {push_master(TYPE, 2);}
   |struct_header '{' struct_arguments '}' {push_master(TYPE, 2);}
;

right_value_arguments:
    '(' ')' {push_master(ARGUMENTS,0);}
   |'(' right_value_argument_list ')'
;
    
right_value_argument_list:
      right_value {push_master(ARGUMENTS,1);}
    | right_value_argument_list ',' right_value {push_master(ARGUMENTS,2);}
;

struct_header:
	struct_type struct_name {push_master(ARGUMENTS, 1);}
   |struct_type {push_integer(NULL_CONSTANT, 0); push_master(RIGHT_VALUE, 1); push_master(ARGUMENTS, 1);}
;

struct_name:
	IDENT {push_string(STRING_CONSTANT, $1); push_master(RIGHT_VALUE, 1);}
   |'*'   {push_string(STRING_CONSTANT, "*"); push_master(RIGHT_VALUE, 1);}
   |'#'   {push_string(STRING_CONSTANT, "#"); push_master(RIGHT_VALUE, 1);}
   
struct_type:
	STRUCT_TOKEN {push_string(IDENTIFIER, "Struct");}

struct_arguments:
	/*empty*/
   |struct_arguments type_access struct_name ';' {push_master(ARGUMENTS,3);}
	
extension:
    /*empty*/ {push_master(EXTENSION,0);}
   | extends_token type {push_master(EXTENSION,2);}

extends_token:
	EXTENDS_TOKEN {push_line();}
   
specification:
    /*empty*/ {push_master(SPECIFICATION,0);}
   | as_token type {push_master(SPECIFICATION,2);}

as_token:
	AS_TOKEN {push_line();}
   
class_definition:
    /*empty*/ {push_master(EXECUTION_BLOCK, 0);push_master(EXECUTION_BLOCK, 0); push_master(CLASS_DEFINITION, 2);}
   |execution_block {push_master(EXECUTION_BLOCK, 0); push_master(CLASS_DEFINITION, 2);}
   |'{' statements ELLIPSIS_TOKEN statements '}' {push_master(CLASS_DEFINITION, 2);}
   
execution_block:
     ';' {push_master(EXECUTION_BLOCK, 0);}
    | statement {push_master(EXECUTION_BLOCK, 1);}
    |'{' statements '}' {push_master(EXECUTION_BLOCK, 1);}
;
        
statements:
    /*empty*/ {push_master(EXECUTION_BLOCK, 0);}
  | statements statement  {push_master(EXECUTION_BLOCK, 2);}
  
statement:
    simple_statement ';'
   |conditional_statement
   |loop
   |for_loop
   |do_loop
   |HEADER_TOKEN {push_line();push_master(HEADER_MARK, 1);}
;

simple_statement:
    local_declarations 
   |declaration
   |field_assignment
   |remove
   |right_value
   |break
   |continue
   |return
;   

break:
    BREAK_TOKEN {push_line(); push_master(BREAK, 1);}
    
continue:
    CONTINUE_TOKEN {push_line(); push_master(CONTINUE, 1);}
    
return:
    return_token {push_integer(NULL_CONSTANT, 0); push_master(RIGHT_VALUE, 1); push_master(RETURN, 2);}
   |return_token right_value {push_master(RETURN, 2);}

return_token:
	RETURN_TOKEN {push_line();}

declaration:
    _declaration {push_integer(SHOWCASED, 0); push_master(DECLARATION, 3);}
   |_declaration SHOWCASED_TOKEN {push_integer(SHOWCASED, 1); push_master(DECLARATION, 3);}
    

_declaration:   
    type_access name_identifier 
   |_declaration '['']' {
                                stash(0);
                                stash(0);
                                push_string(IDENTIFIER, "Array");
                                unstash(0);
                                push_master(ARGUMENTS,1);
                                push_master(TYPE,2);
                                push_master(RIGHT_VALUE,1);
                                unstash(0);
    }
   |_declaration '['right_value']' {
                                stash(1);
                                stash(0);
                                stash(0);
                                push_string(IDENTIFIER, "Tuple");
                                unstash(0);
                                unstash(1);
                                push_master(ARGUMENTS,2);
                                push_master(TYPE,2);
                                push_master(RIGHT_VALUE,1);
                                unstash(0);
    }
;

local_declarations:
	VAR_TOKEN _local_declarations
;
       
_local_declarations:
	local_declaration {push_master(LOCAL_DECLARATIONS, 1);}
   |_local_declarations ',' local_declaration {push_master(LOCAL_DECLARATIONS, 2);}
;
	   
local_declaration:   
    identifier {push_master(LOCAL_DECLARATION, 1);}  
   |identifier '=' right_value{push_master(LOCAL_DECLARATION, 2);}  
;

identifier:
    IDENT {push_string(IDENTIFIER, $1);}
;

extended_identifier:
    IDENT {push_string(IDENTIFIER, $1);}
   |A_IDENT {push_string(IDENTIFIER, $1);}
;

function_identifier:
    P_IDENT {push_string(IDENTIFIER, $1);}

name_identifier:
    IDENT {push_string(IDENTIFIER, $1);}
   |'*'   {push_string(IDENTIFIER, "*");}
   |'#'   {push_string(IDENTIFIER, "#");}
    
right_value:
     right_value '=' right_value                 {handle_op(ASSIGN_OP);}
    |right_value RIGHT_ASSIGN_TOKEN right_value  {handle_op(RIGHT_ASSIGN_OP);}
    |right_value LEFT_ASSIGN_TOKEN right_value   {handle_op(LEFT_ASSIGN_OP);}
    |right_value ADD_ASSIGN_TOKEN right_value    {handle_op(ADD_ASSIGN_OP);}
    |right_value SUB_ASSIGN_TOKEN right_value    {handle_op(SUB_ASSIGN_OP);}
    |right_value MUL_ASSIGN_TOKEN right_value    {handle_op(MUL_ASSIGN_OP);}
    |right_value DIV_ASSIGN_TOKEN right_value    {handle_op(DIV_ASSIGN_OP);}
    |right_value MOD_ASSIGN_TOKEN right_value    {handle_op(MOD_ASSIGN_OP);}
    |right_value AND_ASSIGN_TOKEN right_value    {handle_op(AND_ASSIGN_OP);}
    |right_value XOR_ASSIGN_TOKEN right_value    {handle_op(XOR_ASSIGN_OP);}
    |right_value OR_ASSIGN_TOKEN right_value     {handle_op(OR_ASSIGN_OP);}
    |right_value '?' right_value ':' right_value {handle_op(TERNARY_OP);}
    |right_value OR_TOKEN right_value            {handle_op(OR_OP);}
    |right_value AND_TOKEN right_value           {handle_op(AND_OP);}
    |right_value '|' right_value                 {handle_op(BITWISE_OR_OP);}
    |right_value '^' right_value                 {handle_op(BITWISE_XOR_OP);}
    |right_value '&' right_value                 {handle_op(BITWISE_AND_OP);}
    |right_value EQ_TOKEN right_value            {handle_op(EQ_OP);}
    |right_value NE_TOKEN right_value            {handle_op(NE_OP);}
    |right_value GE_TOKEN right_value            {handle_op(GE_OP);}
    |right_value '>' right_value                 {handle_op(GT_OP);}
    |right_value LE_TOKEN right_value            {handle_op(LE_OP);}
    |right_value '<' right_value                 {handle_op(LT_OP);}
    |right_value RIGHT_TOKEN right_value         {handle_op(RIGHT_OP);}
    |right_value LEFT_TOKEN right_value          {handle_op(LEFT_OP);}
    |right_value '+' right_value                 {handle_op(ADD_OP);}
    |right_value '-' right_value                 {handle_op(SUB_OP);}
    |right_value '*' right_value                 {handle_op(MUL_OP);}
    |right_value '/' right_value                 {handle_op(DIV_OP);}
    |right_value '%' right_value                 {handle_op(MOD_OP);}  
    |NOT_TOKEN right_value                       {handle_op(NOT_OP);}
    |BITWISE_NOT_TOKEN  right_value              {handle_op(BITWISE_NOT_OP);}
    |'-' %prec OPP right_value                   {handle_op(OPP_OP);}
    |INC_TOKEN right_value                       {handle_op(PRE_INC_OP);}
    |DEC_TOKEN right_value                       {handle_op(PRE_DEC_OP);}
    |right_value INC_TOKEN %prec SUF_INC         {handle_op(SUF_INC_OP);}
    |right_value DEC_TOKEN %prec SUF_DEC         {handle_op(SUF_DEC_OP);}
    |constant_value                              {push_master(RIGHT_VALUE, 1);}
    |variable                                    {push_master(RIGHT_VALUE, 1);}
    |explicit_type                               {push_master(RIGHT_VALUE, 1);}
    |function_identifier right_value_arguments   {push_master(FUNCTION_EVALUATION, 2);push_master(RIGHT_VALUE, 1);}
	|variable SUBSCOPE_ASSIGN_TOKEN right_value {push_master(FIELD_ASSIGN, 2);push_master(RIGHT_VALUE, 1);}
    |'('right_value')'                          
;

constant_value:
    int_constant 
   |uint_constant
   |string_constant
   |float_constant
   |null_constant
   |undefined_constant
   |empty_string_constant
;

int_constant : 
    INT_VALUE {push_integer(INT_CONSTANT, $1);};

uint_constant : 
    UINT_VALUE {push_uinteger(UINT_CONSTANT, $1);};

string_constant : 
    STRING_VALUE {push_string(STRING_CONSTANT, $1);};
    
float_constant :
    FLOAT_VALUE {push_float(FLOAT_CONSTANT, $1);}
    
null_constant:
    NULL_TOKEN {push_integer(NULL_CONSTANT, 0);}
	
undefined_constant:
    UNDEFINED_TOKEN {push_integer(UNDEFINED_CONSTANT, 0);}
    
empty_string_constant:
    EMPTY_STRING_TOKEN {push_integer(EMPTY_STRING_CONSTANT, 0);}

variable:
    extended_identifier {push_master(VARIABLE, 1);}
   |'['  ']' {push_integer(NULL_CONSTANT, 0); push_master(RIGHT_VALUE, 1); push_master(VARIABLE, 1);}
   |'[' right_value ']' {push_master(VARIABLE, 1);}
   |variable '.' extended_identifier {push_master(VARIABLE, 2);}
   |variable '[' right_value ']' {push_master(VARIABLE, 2);}
   |variable '[' ']' {push_integer(NULL_CONSTANT, 0); push_master(RIGHT_VALUE, 1); push_master(VARIABLE, 2);}

field_assignment:
	
   |variable SUBSCOPE_ASSIGN_TOKEN field_assignment {push_master(FIELD_ASSIGN, 2);}

remove:
	REMOVE_TOKEN variable {push_master(REMOVE, 1);}

conditional_statement:
    if_token '(' right_value ')' execution_block {push_master(EXECUTION_BLOCK,0); push_master(CONDITIONAL_STATEMENT,4);}
   |if_token '(' right_value ')' execution_block ELSE_TOKEN execution_block{push_master(CONDITIONAL_STATEMENT,4);}

if_token:
	IF_TOKEN {push_line();}
   
loop:
    while_token '(' right_value ')' execution_block {push_master(LOOP,3);}

while_token:
	WHILE_TOKEN {push_line();}
	
for_loop:
    for_token '(' simple_statement ';' right_value ';' simple_statement ')'{stash(0);} execution_block {unstash(0); push_master(EXECUTION_BLOCK,2); push_master(LOOP,2); push_master(EXECUTION_BLOCK,3);}

for_token:
	FOR_TOKEN {push_line();}
	
do_loop:
    do_token execution_block {stash(0);} WHILE_TOKEN '(' right_value ')' {unstash(0); push_master(DO_LOOP, 3);}
	
do_token:
	DO_TOKEN {push_line();}
%%

void yyerror(char const *s)
{
    fprintf(stderr, "%s on line %d\n", s, line_number);
}


int main(int argc, char *argv[])                                                                             
{    
    if(argc > 1)
    {
        // open a file handle to a particular file:
        FILE *file = fopen(argv[1], "r");
        printf("input: %s\n", argv[1]);
        // make sure it is valid:
        if (!file) 
        {
            fprintf(stderr,"Can't open file %s",argv[1]);
            return EXIT_FAILURE;
        }
        // set flex to read from it instead of defaulting to STDIN:
        yyin = file;
        
        // parse through the input until there is no more:
        do 
        {
            yyparse();
        } while (!feof(yyin));
        
        FILE *output;
        
        if(argc > 2)
        {
            output = fopen(argv[2], "wb");
            printf("output: %s\n", argv[2]);
        }
        else
        {
            output = fopen("output.hmc", "wb");
            printf("output: %s\n", "output.hmc");
        }
        

        int i;
        for(i = 0; i < headerSize; ++i)
            fputc(header[i], output);
        
#ifdef STACK_DEBUG
		dump_stack();
#endif
		
        while(!empty()) 
            pop();
        
        write_node(output, &root);
		root.id = DEBUG;
		root.size = 0;
		root.line_number = -1;
		root.parent = NULL;
		root.first_child.node = NULL;
		root.next_sibling = NULL;
		
		while(!empty()) 
            pop();
        
        write_node(output, &root);
		
        return EXIT_SUCCESS;
        
    }
    else
    {
        fprintf(stderr,"No input specified");
        return EXIT_FAILURE;
    }
}

