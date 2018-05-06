%{
#include <cstdio>
#include <iostream>
#include <bits/stdc++.h>
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

/* Global Data */

// Output file stream
ofstream out("output.class");

// Types enum
enum typeEnum {
    intType, floatType, errorType 
};

// Symbol table <name, <variable number, type>>
map<string, pair<int,typeEnum> > symbolTable;

int labelCnt = 0; // Gives incremental index to labels
int varCnt = 0;   // Gives incremental address to variables
int lineCnt = 0;  // Gives the next line number in output file

/*
// A map to put the correct java byte code instruction
// for relop and arithmetic operations
map<string,string> instruction = {
    {"==", "if_icmpeq"},
    {"<=", "if_icmple"},
    {">=", "if_icmpge"},
    {"!=", "if_icmpne"},
    {">",  "if_icmpgt"},
    {"<",  "if_icmplt"},

    {"+", "add"},
    {"-", "sub"},
    {"/", "div"},
    {"*", "mul"},
    {"%", "rem"}    
};
*/


/* Function Headers */

void print(string s);
void defineVariable(char *id_val, int type);
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

    char *next;

    int type;

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


%type <type> primitive_type
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
    {
        defineVariable($2, $1);
    }
    ;
primitive_type:
    INT { $$ = intType; }
    | FLOAT { $$ = floatType; }
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

void print(string s){
    out << s;
}

void defineVariable(char *id_val, int type){
    string str = string(id_val);
    if(symbolTable.count(str))
        yyerror("Error: Multiple definitions of variable.");
    symbolTable[str] = make_pair(++varCnt, (typeEnum) type);
}