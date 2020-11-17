%option nounput
%option noinput
%option noyywrap
%{
#include "sm.tab.h"
int line_num = 1;
%}
%%
[ \t]	;
"initial"       {return INIT;}
"state"	{return STATE;}
"exit"		{return EXIT;}
"entry"	{return ENTRY;}
"on"		{return ON;}
"action"	{return ACTION;}
"goto"		{return GOTO;}
"{"		{return OPEN_BRAKET;}
"}"		{return CLOSE_BRAKET;}
[a-zA-Z0-9_]+	{
			yylval.sval = strdup(yytext);
			return ITEMNAME;
		}
\n              {++line_num;}
.		;
%%

