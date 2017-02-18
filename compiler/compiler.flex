/*This file is part of the HexaMonkey project, a multimedia analyser
 *Copyright (C) 2013  Sevan Drapeau-Martin, Nicolas Fleury

 *This program is free software; you can redistribute it and/or
 *modify it under the terms of the GNU General Public License
 *as published by the Free Software Foundation; either version 2
 *of the License, or (at your option) any later version.

 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.

 *You should have received a copy of the GNU General Public License
 *along with this program; if not, write to the Free Software
 *Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.*/

D			[0-9]
L			[_a-zA-Z]
DL          [_a-zA-Z0-9]
H			[a-fA-F0-9]
E			[Ee][+-]?{D}+
WS          [ \t\r]

%{                                                                                          
#include "compiler.tab.h"
#include <string.h>      
#include "struct.h"                                                               
extern int line_number;                                                              
%}                                                                                          
%option noyywrap                                                                            
 
%%  

"//".*"\n"	/*comment*/{++line_number;}
"/*"	   {comment();}

\"\" {yylval.s = strdup(""); return EMPTY_STRING_TOKEN;}
\'\' {yylval.s = strdup(""); return EMPTY_STRING_TOKEN;}
\"(\\.|[^\\"])+\" {yylval.s = strdup(yytext+1); yylval.s[strlen(yylval.s)-1] = '\0'; return STRING_VALUE;}
\'(\\.|[^\\'])+\' {yylval.s = strdup(yytext+1); yylval.s[strlen(yylval.s)-1] = '\0'; return STRING_VALUE;}
({H}{H}|"xx")({WS}({H}{H}|"xx"))+ {yylval.s = strdup(yytext); return MAGIC_NUMBER;}

">>="			{return(RIGHT_ASSIGN_TOKEN);}
"<<="			{return(LEFT_ASSIGN_TOKEN);}
"+="			{return(ADD_ASSIGN_TOKEN);}
"-="			{return(SUB_ASSIGN_TOKEN);}
"*="			{return(MUL_ASSIGN_TOKEN);}
"/="			{return(DIV_ASSIGN_TOKEN);}
"%="			{return(MOD_ASSIGN_TOKEN);}
"&="			{return(AND_ASSIGN_TOKEN);}
"^="			{return(XOR_ASSIGN_TOKEN);}
"|="			{return(OR_ASSIGN_TOKEN);}
">>"			{return(RIGHT_TOKEN);}
"<<"			{return(LEFT_TOKEN);}
"++"			{return(INC_TOKEN);}
"--"			{return(DEC_TOKEN);}
"&&"			{return(AND_TOKEN);}
"||"			{return(OR_TOKEN);}
"<="			{return(LE_TOKEN);}
">="			{return(GE_TOKEN);}
"=="			{return(EQ_TOKEN);}
"!="			{return(NE_TOKEN);}
"..."           {return(ELLIPSIS_TOKEN);}
"---"           {return(HEADER_TOKEN);}
":="            {return(SUBSCOPE_ASSIGN_TOKEN);}
"!"             {return(NOT_TOKEN);}
"~"             {return(BITWISE_NOT_TOKEN);}

[;{}()\[\],&|+/\-*<>=.%#?:] {return *yytext;} 

"0b"[01]+    {yylval.u = strtoull(yytext+2, NULL, 2); return UINT_VALUE;}
0[xX]{H}+	{yylval.u = strtoull(yytext,NULL,0); return UINT_VALUE;}
{D}+		{yylval.u = strtoull(yytext,NULL,0); return UINT_VALUE;}

{D}+{E}		    {yylval.f = atof(yytext); return FLOAT_VALUE;}
{D}*"."{D}+({E})?	{yylval.f = atof(yytext); return FLOAT_VALUE;}
{D}+"."{D}*({E})?	{yylval.f = atof(yytext); return FLOAT_VALUE;}

"class"     {return CLASS_TOKEN;}
"extends"   {return EXTENDS_TOKEN;}
"as"        {return AS_TOKEN;}
"forward"   {return FORWARD_TOKEN;}
"to"        {return TO_TOKEN;}
"for"       {return FOR_TOKEN;}
"while"     {return WHILE_TOKEN;}
"do"        {return DO_TOKEN;}
"if"        {return IF_TOKEN;}
"else"      {return ELSE_TOKEN;}
"return"    {return RETURN_TOKEN;}
"break"     {return BREAK_TOKEN;}
"continue"  {return CONTINUE_TOKEN;}
"var"       {return VAR_TOKEN;}  
"function"  {return FUNCTION_TOKEN;}  
"const"     {return CONST_TOKEN;}  
"showcased" {return SHOWCASED_TOKEN;}
"null"      {return NULL_TOKEN;}
"undefined" {return UNDEFINED_TOKEN;}
"delete"    {return REMOVE_TOKEN;}
"self"      {return SELF_TOKEN;}
"with"      {return WITH_TOKEN;}
"virtual"   {return VIRTUAL_TOKEN;}

"import"            {return IMPORT_TOKEN;}
"addMagicNumber"    {return ADD_MAGIC_NUMBER_TOKEN;}
"addExtension"      {return ADD_EXTENSION_TOKEN;}
"addSyncbyte"       {return ADD_SYNCBYTE_TOKEN;}

"struct"    {return STRUCT_TOKEN;}
"enum"      {return ENUM_TOKEN;}

"=>"        {return ASSOC_TOKEN;}       

{L}{DL}* {yylval.s = strdup(yytext); return IDENT;} 
"@"{L}{DL}* {yylval.s = strdup(yytext); return A_IDENT;} 
{WS}+      /* eat up whitespace */   
[\n]     {++line_number;}                                                                     
%%

comment()
{
	char c, c1;

	while(1)
	{
		while ((c = input()) != '*' && c != 0)
		{
			if(c == '\n')
				++line_number;
		}

		if ((c1 = input()) != '/' && c != 0)
		{
			unput(c1);
		}
		else
		{
			break;
		}
	}
}
