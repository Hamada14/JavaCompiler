%{
#include <iostream>
#include <string.h>
#include <vector>
using namespace std;
#define YY_DECL extern "C" int yylex()

#include "parser.tab.h"

%}

DIGIT    [0-9]
LETTER   ([a-z]|[A-Z])
DIGITS   [0-9]+

%%
[ \t\n]          ;

[\(\)\{\},;]               { return yytext[0]; }
int                         { return INT; }
float                       { return FLOAT; }
boolean                     { return BOOLEAN; }
if                          { return IF; }
else                        { return ELSE; }
while                       { return WHILE; }
for                         { return FOR; }
{LETTER}[{LETTER}{DIGIT}]*  { yylval.id_val = strdup(yytext); return ID; }
{DIGITS}                    { yylval.ival = atoi(yytext); return INT_VAL;}
{DIGITS}"."{DIGITS}         { yylval.fval = atof(yytext); return FLOAT_VAL; }

"=="|"!="|">"|">="|"<"|"<=" { yylval.relop = strdup(yytext); return RELOP; }
[\=]                        { yylval.assign = yytext[0]; return ASSIGN; }
[\+\-]                      { yylval.addop = yytext[0]; return ADDOP; }
[\*/]                       { yylval.mulop = yytext[0]; return MULOP; }
.                           { ;}
%%


//Remember to handle floats with all formats