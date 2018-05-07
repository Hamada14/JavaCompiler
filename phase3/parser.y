%{
#include <bits/stdc++.h>
using namespace std;

// stuff from flex that bison needs to know about:
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;

/* Global Data */

const int MAX_LABEL_LENGTH = 10;

// Output file stream
ofstream out("output.class");

// Output lines
vector<string> code;

// Types enum
enum typeEnum {
    intType, floatType, errorType 
};

// Symbol table <name, <variable number, type>>
map<string, pair<int,typeEnum> > symbolTable;

int labelCnt = 0; // Gives incremental index to labels
int varCnt = 0;   // Gives incremental address to variables
int lineCnt = 0;  // Gives the next line number in output file

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


/* Function Headers */

void print();
void addLine(string s);
void log(string s);
void defineVariable(char *id_val, int type);
void yyerror(const char *s);
char* newLabel();
int getType(int t1, int t2);

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

    // These containers contain indexes in 'code' vector.
    //vector<int> next, trueNext, falseNext;
    /*
    if
    expr
    goto true
    goto false
    true: ...
    goto next
    false: ...
    next: ...
    */

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


%type <type> primitive_type factor term
%type <type> expression simple_expression
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
    INT
    {
        $$ = intType;
    }
    | FLOAT
    { 
        $$ = floatType;
    }
    ;
if:
    IF '(' expression ')' '{' statement '}' ELSE '{' statement '}'
    {

    }
    ;
while:
    WHILE '(' expression ')' '{' statement '}'
    ;
assignment:
    ID ASSIGN expression ';'
    {
        // Cosider casting instead of the following.
        if(symbolTable[$1].second != $3)
            yyerror("Assigned a variable to an expression with different type.");
        if(symbolTable[$1].second == intType)
            addLine("istore " + to_string(symbolTable[$1].first));
        else
            addLine("fstore " + to_string(symbolTable[$1].first));
    }
    ;
expression:
    simple_expression
    /* to-do or ask about
    | simple_expression RELOP simple_expression
    {
        $$ = getType($1, $2);   
    }
    */
    ;
simple_expression:
    term
    | sign term
    { 
        $$ = $2;
    }
    | simple_expression ADDOP term
    {
        $$ = getType($1, $3);
        if($$ == intType)
            addLine("i" + instruction[string(1, $2)]);
        else
            addLine("f" + instruction[string(1, $2)]);
    }
    ;
term:
    factor
    | term MULOP factor
    {
        $$ = getType($1, $2);
        if($$ == intType)
            addLine("imul");
        else
            addLine("fmul");
    }
    ;
factor:
    ID
    { 
        $$ = symbolTable[$1].second;
        if(symbolTable[$1].second == intType)
            addLine("iload_" + to_string(symbolTable[$1].first));
        else
            addLine("fload_" + to_string(symbolTable[$1].first));
    }
    | INT_VAL
    { 
        $$ = intType;
        addLine("ldc " + to_string($1));
    }
    | FLOAT_VAL
    { 
        $$ = floatType;
        addLine("ldc " + to_string($1));
    }
    | '(' expression ')'
    { 
        $$ = $2;
    }
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

    print();
}

void yyerror(const char *s) {
	cout << "EEK, parse error!  Message: " << s << endl;
	// might as well halt now:
	exit(-1);
}

void print(){
    int i = 0;
    for(string s : code) 
        out <<i++ <<": " <<s << endl;
}

void log(string s){
    cout <<s <<endl;
}

void defineVariable(char *id_val, int type){
    string str = string(id_val);
    if(symbolTable.count(str))
        yyerror("Multiple definitions of variable.");
    symbolTable[str] = make_pair(++varCnt, (typeEnum) type);
    log("Declared variable " + str + " with type " + (type ? "float" : "int") + " and variable number " + to_string(varCnt));
}


char* newLabel(){
    char *ret = (char *) malloc(MAX_LABEL_LENGTH);
    memcpy(ret, ("L_" + to_string(labelCnt++)).c_str(), MAX_LABEL_LENGTH);
    return ret;
}

void addLine(string s){
    code.push_back(s);
}

int getType(int t1, int t2){
    return max(t1, t2);
}