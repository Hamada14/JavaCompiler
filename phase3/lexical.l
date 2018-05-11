%{
#include <iostream>
#include <string.h>
#include <vector>
#include <algorithm>
#include <cstring>

using namespace std;
#define YY_DECL extern "C" int yylex()
#include "parser.tab.h"
bool lexical_error;
int cur_line = 1;

void log_error(string error);
char* fix_float(char* init_str);
string removeLeadingZeros(string str);
string normalize(string int_val, string decimal_val, string exp_val);
%}

DIGIT    [0-9]
LETTER   ([a-z]|[A-Z])
DIGITS   [0-9]+

%%
[ \t] ;
[\n]                                                           { cur_line++; }

[\(\)\{\},;]                                                      { return yytext[0]; }
int                                                               { return INT; }
float                                                             { return FLOAT; }
boolean                                                           { return BOOLEAN; }
if                                                                { return IF; }
else                                                              { return ELSE; }
while                                                             { return WHILE; }
for                                                               { return FOR; }
true															                                { return TRUE; }
false															                                { return FALSE; }
{LETTER}({LETTER}|{DIGIT}|"_")*                                   { yylval.id_val = strdup(yytext); return ID; }
{DIGITS}                                                          { yylval.ival = strdup(yytext); return INT_VAL;}
(({DIGIT}*\.{DIGITS})|({DIGITS}))(E[\+\-]?{DIGITS})?              { yylval.fval = fix_float(strdup(yytext)); return FLOAT_VAL; }
"&&"                                                              { yylval.andoperator = strdup(yytext); return AND_OPERATOR; }
"||"                                                              { yylval.oroperator = strdup(yytext); return OR_OPERATOR; }
"!"                                                               { yylval.notoperator = strdup(yytext); return NOT_OPERATOR; }
"=="|"!="|">"|">="|"<"|"<="                                       { yylval.relop = strdup(yytext); return RELOP; }
[\=]                                                              { yylval.assign = yytext[0]; return ASSIGN; }
[\+\-]                                                            { yylval.addop = yytext[0]; return ADDOP; }
[\*/]                                                             { yylval.mulop = yytext[0]; return MULOP; }
.                                                                 { log_error("Couldn't match text{" + string(yytext) + "} at line:" + to_string(cur_line));
                                                                    log_error("Skipping match{" + string(yytext) + "}, Program might be malformed.");}
%%


//Util functions

void log_error(string error) {
  cout << "Error: " << error << endl;
}

char* fix_float(char* init_ptr) {
    string init_str(init_ptr);
    size_t dot_idx = init_str.find(".");
    size_t e_idx = init_str.find("E");
    string int_val, decimal_val, exp_val;
    if(dot_idx == string::npos) {
        int_val = init_str.substr(0, e_idx == string::npos ? init_str.length() : e_idx);
        decimal_val = "0";
    } else {
        int_val = dot_idx == 0 ? "0" : init_str.substr(0, dot_idx);
        int next_end = e_idx == string::npos ? init_str.length() : e_idx;
        decimal_val = init_str.substr(dot_idx + 1, next_end - dot_idx - 1);
    }
    exp_val = e_idx == string::npos ? "0" : init_str.substr(e_idx + 1);
    string result = normalize(int_val, decimal_val, exp_val);
    result = removeLeadingZeros(result);
    char* result_ptr = (char*)malloc(sizeof(result.length()) + 1);
    strcpy(result_ptr, result.c_str());
    return result_ptr;
}

string removeLeadingZeros(string str) {
    int start_index = 0;
    if(str.find(".") == string::npos) {
        return str;
    }
    if(str[0] == '.') {
        return "0" + str;
    } else {
        while(str[start_index] == '0' && str[start_index + 1] != '.') {
            start_index++;
        }
        return str.substr(start_index);
    }
}

string normalize(string int_val, string decimal_val, string exp_val) {
    int exp = stoi(exp_val);
    string result;
    bool is_reversed = exp < 0;
    if(is_reversed) {
        reverse(int_val.begin(), int_val.end());
        reverse(decimal_val.begin(), decimal_val.end());
        swap(int_val, decimal_val);
    }
    int ptr = 0;
    exp = abs(exp);
    while(exp) {
        exp--;
        int_val += ptr < decimal_val.length() ? decimal_val[ptr++] : '0';
    }
    result = int_val + "." + (ptr < decimal_val.length() ? decimal_val.substr(ptr) : "0");
    if(is_reversed) {
        reverse(result.begin(), result.end());
    }
    return result;
}
//Remember to handle floats with all formats
