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

// Maps to put the correct java byte code instruction
// for relop and arithmetic operations
map<string,string> intInstruction = {
    {"==", "if_icmpeq"},
    {"<=", "if_icmple"},
    {">=", "if_icmpge"},
    {"!=", "if_icmpne"},
    {">",  "if_icmpgt"},
    {"<",  "if_icmplt"}
};
map<string,string> floatInstruction = {
    {"==", "ifeq"},
    {"<=", "ifle"},
    {">=", "ifge"},
    {"!=", "ifne"},
    {">",  "ifgt"},
    {"<",  "iflt"}
};
map<string,string> arithmeticInstruction = {
    {"+", "add"},
    {"-", "sub"},
    {"/", "div"},
    {"*", "mul"}
};


/* Function Headers */

void print();
void addLine(string s);
void log(string s);
void defineVariable(char *id_val, int type);
void yyerror(const char *s);
char* newLabel();
int getType(int t1, int t2);
void addAddress(vector<int> *v, int a);

%}

// Bison fundamentally works by asking flex to get the next token, which it
// returns as an object of type "yystype".  But tokens could be of any
// arbitrary data type!  So we deal with that in Bison by defining a C union
// holding each of the types of tokens that Flex could return, and have Bison
// use that union instead of "int" for the definition of "yystype":
%union {
  int mark_val;
  char *ival;
  char *fval;
  char *sval;
  char *id_val;
  struct container {
    // These containers contain indexes in 'code' vector.
    vector<int> *next, *trueList, *falseList;
    int type;
  } container;
  int type;
  char addop;
  char mulop;
  char assign;
  char *relop;
  char *andoperator;
  char *oroperator;
  char *notoperator;
}

// define the constant-string tokens:
%token INT FLOAT BOOLEAN
%token IF ELSE
%token FOR WHILE
%token TRUE FALSE

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
%token <andoperator> AND_OPERATOR
%token <oroperator> OR_OPERATOR
%token <notoperator> NOT_OPERATOR


%type <type> primitive_type factor term
%type <type> simple_expression
%type <container> if statement while for boolean_expression
%type <mark_val> mark
%type <addop> sign
%%



// Rule definition
method_body:
    statement_list
    { addLine("return"); }
    ;
statement_list:
    statement
    | statement_list statement
    ;
statement:
    declaration { $$.next = new vector<int>; }
    | if { addAddress($1.next, code.size()); }
    | while { addAddress($1.next, code.size()); }
    | assignment { $$.next = new vector<int>; }
    | for { addAddress($1.next, code.size()); }
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
    IF '(' boolean_expression ')' '{'
    {
        addAddress($3.trueList, code.size());
    }
    statement
    {
        $7.next = new vector<int>;
        $7.next->push_back(code.size());
        addLine("goto ");
    }
    '}' ELSE '{'
    {
       addAddress($3.falseList, code.size());
    }
    statement '}'
    {
        $$ = $7;
        if($$.next == nullptr)
            $$.next = new vector<int>;
        if($13.next){
            while($13.next->size()){
                int i = $13.next->back();
                $$.next->push_back(i);
                $13.next->pop_back();
            }
        }
    }
    ;
while:
    WHILE '(' mark boolean_expression ')' '{'
    {
        addAddress($4.trueList, code.size());
    }
    statement '}'
    {
        addLine("goto " + to_string($3));
        $$.next = $4.falseList;
        if($$.next == nullptr)
            $$.next = new vector<int>;
    }
    ;
mark:
    {
        $$ = code.size();
    }
    ;
for:
    FOR '(' assignment mark boolean_expression ';' mark assignment ')' '{'
    {
        addLine("goto " + to_string($4));
        addAddress($5.trueList, code.size());
    }
    statement '}'
    {
        addLine("goto " + to_string($7));
        $$.next = $5.falseList;
        if($$.next == nullptr)
            $$.next = new vector<int>;
    }
    ;
assignment:
    ID ASSIGN simple_expression ';'
    {
        if(!symbolTable.count(string($1)))
            yyerror("Undeclared variable.");
        // Consider casting instead of the following.
        if(symbolTable[string($1)].second != $3)
            yyerror("Assigned a variable to an expression with different type.");
        if(symbolTable[string($1)].second == intType)
            addLine("istore " + to_string(symbolTable[string($1)].first));
        else
            addLine("fstore " + to_string(symbolTable[string($1)].first));
    }
    ;
boolean_expression:
    TRUE
    {
        $$.trueList = new vector<int>;
        $$.falseList = new vector<int>;
        $$.trueList->push_back(code.size());
        addLine("goto ");
    }
    | FALSE
    {
        $$.trueList = new vector<int>;
        $$.falseList = new vector<int>;
        $$.falseList->push_back(code.size());
        addLine("goto ");
    }
    | simple_expression RELOP simple_expression
    {
        if($1 != $3) yyerror("Comparing two expressions of different types.");
        $$.trueList = new vector<int>;
        $$.falseList = new vector<int>;
        if($1 == intType){
            $$.trueList->push_back(code.size());
            addLine(intInstruction[string($2)] + " ");
            $$.falseList->push_back(code.size());
            addLine("goto ");
        }
        else{
            addLine("fcmpl");
            $$.trueList->push_back(code.size());
            addLine(floatInstruction[string($2)] + " ");
            $$.falseList->push_back(code.size());
            addLine("goto ");
        }
    }
    | boolean_expression AND_OPERATOR mark boolean_expression
    {
        addAddress($1.trueList, $3);
        $$.falseList = $1.falseList;
        while($4.falseList->size()){
            int i = $4.falseList->back();
            $$.falseList->push_back(i);
            $4.falseList->pop_back();
        }
        $$.trueList = $4.trueList;
    }
    | boolean_expression OR_OPERATOR mark boolean_expression
    {
        addAddress($1.falseList, $3);
        $$.trueList = $1.trueList;
        while($4.trueList->size()){
            int i = $4.trueList->back();
            $$.trueList->push_back(i);
            $4.trueList->pop_back();
        }
        $$.falseList = $4.falseList;
    }
    | NOT_OPERATOR boolean_expression
    {
        $$.trueList = $2.falseList;
        $$.falseList = $2.trueList;
    }
    ;
simple_expression:
    term
    | sign term
    {
        $$ = $2;
        if ($1 == '-') {
          addLine("iconst_m1");
          if ($2 == intType)
            addLine("imul");
          else
            addLine("fmul");
        }
    }
    | simple_expression ADDOP term
    {
        $$ = getType($1, $3);
        if($$ == intType)
            addLine("i" + arithmeticInstruction[string(1, $2)]);
        else
            addLine("f" + arithmeticInstruction[string(1, $2)]);
    }
    ;
term:
    factor
    | term MULOP factor
    {
        $$ = getType($1, $3);
        if($$ == intType)
            addLine("i" + arithmeticInstruction[string(1, $2)]);
        else
            addLine("f" + arithmeticInstruction[string(1, $2)]);
    }
    ;
factor:
    ID
    {
        if(!symbolTable.count(string($1)))
          yyerror("Undeclared variable.");
        $$ = symbolTable[string($1)].second;
        if(symbolTable[$1].second == intType)
            addLine("iload_" + to_string(symbolTable[string($1)].first));
        else
            addLine("fload_" + to_string(symbolTable[string($1)].first));
    }
    | INT_VAL
    {
        $$ = intType;
        addLine("sipush " + string($1));
    }
    | FLOAT_VAL
    {
        $$ = floatType;
        addLine("ldc " + string($1));
    }
    | '(' simple_expression ')'
    {
        $$ = $2;
    }
    ;
sign:
    ADDOP
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

void addAddress(vector<int> *v, int a){
    if(v == nullptr) return;
    while((*v).size()){
        code[(*v).back()] += to_string(a);
        (*v).pop_back();
    }
}
