%{					//definitions
#include<cstring>
//#include"2.h"				//include header file
int yylex(void);
void yyerror(char* s);
extern char *yytext;
string arr[10];int count=-1;		//string array to store predicates
#define YYDEBUG_LEXER_TEXT yytext
%}
%union {				//Union which will be transformed into struct by YACC
	int ival;
	float fval;
	Predicates *ps;
	Predicate *p;
	char* sval;
	Relation* rel;
	RAop* e;
};
%token SELECTION PROJECTION RENAME CROSSPDT UNION MINUS OPEND //tokens
%token <ival> INT
%token <fval> FLOAT
%token <sval> NAME			//tokens assigned to types
%token <sval> STRING

%left '^' '$' '~'			//order of precedence and associativity
%left GE LE LT GT 
%left NE EQ
%left  '(' ')'

%type <ps> predicates			//rules assigned types
%type <p> predicate
%type <sval> set
%type <e>  Selection exp
%type <rel> Start
%%

Start:exp 					{$$ = $1->evaluate();int fileno= fn()-1;
						int c=1;string st="";stringstream ss;string cmd="";string output="out.dat";
						while(c<fileno)
						{
							ss << c; ss >> st;
							cmd = "rm " + st + ".dat";
							system(&cmd[0]);c++;ss.clear();st.clear();cmd.clear();
						}
						ss << c; ss >> st;
						cmd = st + ".dat";
						int result = rename(&cmd[0],&output[0]);
						}

exp: '('exp')'					{$$ = $2;}
	|NAME					{$$ = new BaseRelop(($1));}

	|exp CROSSPDT exp			{$$ = new CrossProduct ((*$1),(*$3));}

	|exp UNION exp				{$$ = new Union ((*$1),(*$3));}

	|exp MINUS exp				{$$ = new Minus ((*$1),(*$3));}
	
	|RENAME NAME OPEND '('exp')'		{$$=new Rename ((*$5),$2,"");}
								
	|RENAME NAME '('set')' OPEND '('exp')'	{$$ = new Rename((*$8),$2,arr[count]);arr[count].clear();count--;}
							
	|PROJECTION set OPEND '('exp ')'	{$$ = new Projection ((*$5),arr[count]);arr[count].clear();count--;}
	
	|Selection				{$$=$1;}
	
set: set','NAME					{stringstream ss; string st=""; ss << $3; ss >> st;arr[count] = arr[count]+','+st;}
	|NAME 					{count++;stringstream ss;string st="";ss << $1; ss >> st;arr[count]+= st;}

Selection: SELECTION predicates OPEND '('exp')' 	{ $$ = new Select((*$5),$2);}
	|SELECTION predicate OPEND '('exp')'  	{$$ = new Select((*$5),$2); }
	;

predicates:predicates '^' predicate	{ $$ = new binaryPredicates($1,"^",$3);}
	|predicates '$' predicate	{ $$ = new binaryPredicates($1,"$",$3);}
	|'~' predicates			{ $$ = new unaryPredicates($2,"~");}
	|predicate '^' predicate	{ $$ = new binaryPredicates($1,"^",$3);}
	|predicate '$' predicate	{ $$ = new binaryPredicates($1,"$",$3);}
	|'~' predicate			{ $$ = new unaryPredicates($2,"~");}
	|'(' predicates ')'		{ $$ = $2;}
	|predicate '$' predicates	{ $$ = new binaryPredicates($1,"$",$3);}
	|predicate '^' predicates	{ $$ = new binaryPredicates($1,"^",$3);}
	;
	
predicate:NAME EQ NAME		{ $$ = new Predicate($1,"=",$3,1);}
	|NAME GT NAME		{ $$ = new Predicate($1,">",$3,1);}
	|NAME LT NAME		{ $$ = new Predicate($1,"<",$3,1);}
	|NAME NE NAME		{ $$ = new Predicate($1,"!=",$3,1);}
	|NAME GE NAME		{ $$ = new Predicate($1,">=",$3,1);}
	|NAME LE NAME		{ $$ = new Predicate($1,"<=",$3,1);}
	|NAME EQ INT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"=",temp,2);}
	|NAME GT INT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,">",temp,2);}
	|NAME LT INT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"<",temp,2);}
	|NAME NE INT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"!=",temp,2);}
	|NAME GE INT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,">=",temp,2);}
	|NAME LE INT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"<=",temp,2);}
	|NAME EQ FLOAT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"=",temp,3);}
	|NAME GT FLOAT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,">",temp,3);}
	|NAME LT FLOAT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"<",temp,3);}
	|NAME NE FLOAT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"!=",temp,3);}
	|NAME GE FLOAT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,">=",temp,3);}
	|NAME LE FLOAT		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"<=",temp,3);}
	|NAME EQ STRING		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"=",temp,4);}
	|NAME GT STRING		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,">",temp,4);}
	|NAME LT STRING		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"<",temp,4);}
	|NAME NE STRING		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"!=",temp,4);}
	|NAME GE STRING		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,">=",temp,4);}
	|NAME LE STRING		{ string temp="";stringstream ss;ss << $3;ss >> temp; $$ = new Predicate($1,"<=",temp,4);}
	|'('predicate')'	{ $$ = $2;}
	;
	
/*ENDLS: ENDLS ENDL
      	|ENDL 	
      	;*/
%%
#include<stdio.h>
extern int  yylex();
extern int yyparse();
extern FILE* yyin;

main() {
	// open a file handle to a particular file:
	FILE *myfile = fopen("RAop.inp", "r");
	// make sure it is valid:
	if (!myfile) {
		cout << "I can't RAop.inp!" << endl;
		return -1;
	}
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;
	// parse through the input until there is no more:
	yydebug = 1;
	do {
		yyparse();
	}while (!feof(yyin));

}
void yyerror(char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}

	 
