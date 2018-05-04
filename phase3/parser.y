%{
#include <cstdio>
#include <iostream>
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);
%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
	int ival;
	float fval;
	char *sval;

    char *id_val;

    char addop;
    char mulop;
    char assign;

    char *relop;
}

// define the constant-string tokens:
%token INT FLOAT BOOLEAN
%token IF ELSE
%token FOR WHILE

// define the "terminal symbol" token types I'm going to use (in CAPS
// by convention), and associate each with a field of the union:
%token <ival> INT_VAL
%token <fval> FLOAT_VAL
%token <sval> STRING
%token <id_val> ID
%token <addop> ADDOP
%token <mulop> MULOP
%token <relop> RELOP
%token <assign> ASSIGN
%%



// Rule definition
method_body:
    statement_list
    ;
statement_list:
    statement
    | statement_list statement
    ;
statement:
    declaration
    | if
    | while
    | assignment
    ;
declaration:
    primitive_type ID ';'
    ;
primitive_type:
    INT
    | FLOAT
    ;
if:
    IF '(' expression ')' '{' statement '}' ELSE '{' statement '}'
    ;
while:
    WHILE '(' expression ')' '{' statement '}'
    ;
assignment:
    ID ASSIGN expression ';'
    ;
expression:
    simple_expression
    | simple_expression RELOP simple_expression
    ;
simple_expression:
    term
    | sign term
    | simple_expression ADDOP term
    ;
term:
    factor
    | term MULOP factor
    ;
factor:
    ID
    | INT_VAL
    | FLOAT_VAL
    | '(' expression ')'
    ;
sign:
    '+'
    | '-'
    ;
%%

int main(int, char**) {
	// open a file handle to a particular file:
	FILE *myfile = fopen("input_program.txt", "r");
	// make sure it's valid:
	if (!myfile) {
		cout << "couldn't open file!" << endl;
		return -1;
	}
	// set lex to read from it instead of defaulting to STDIN:
	yyin = myfile;

	// parse through the input until there is no more:
	do {
		yyparse();
	} while (!feof(yyin));

}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}
