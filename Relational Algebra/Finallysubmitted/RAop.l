%{				//definitions
#include <iostream>
#include <cstring>
#include"2.h"			//Include header file
#include"y.tab.h"		//Include file created by YACC
using namespace std;
void yyerror(char*);
%}
				
%%
select[ \t]*"{"			{return SELECTION;}
project[ \t]*"<"		{return PROJECTION;}
rename[ \t]*"<"			{return RENAME;}
[ \t]+X[ \t]+			{return CROSSPDT;}
[ \t]+U[ \t]+			{return UNION;}
[ \t]+"-"[ \t]+			{return MINUS;}
[a-zA-Z][a-zA-Z0-9_\.]*		{char* res = new char[strlen(yytext)+1];
				strcpy(res,yytext);yylval.sval=res;return NAME;}
[ \t]+				;
"}"				{return OPEND;}
">"				{return OPEND;}
[ \t]+">"[ \t]+			{return GT;}
[ \t]+"<"[ \t]+			{return LT;}
"("				{return *yytext;}
")"				{return *yytext;}
,				{return *yytext;}
=				{return EQ;}
">="				{return GE;}
"<="				{return LE;}
"!="				{return NE;}
[\^~$]				{return *yytext;}
[0-9]+\.[0-9]+			{yylval.fval = atof(yytext);return FLOAT;}
[0-9]+				{yylval.ival = atoi(yytext);return INT;}
'[^']*'				{char *res = new char[strlen(yytext) - 1]; 
				strncpy(res,++yytext,yyleng-2);res[yyleng-2]=0;yylval.sval = res; 
				return STRING;}
.				{ECHO ;	cout<<"unexpected\n";}				
[\n]+				;
%%
				
int yywrap(void) {	
return 1;
}
