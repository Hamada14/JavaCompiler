#include "LanguageSpecParser.hpp"


std::map<char, int> LanguageSpecParser::OPERATOR_PRECEDENCE =
{ {'-', 5}, {'*', 4}, {'+', 3}, {'&', 2}, {'|', 1} };
const std::set<char> LanguageSpecParser::VALID_OPERATORS = {'-', '*', '+', '|'};
const std::set<char> LanguageSpecParser::RESERVED_SYM = {'-', '*', '+', '|', '\\', '(', ')', '=', ':'};

NFA* LanguageSpecParser::parseRegularExpression(std::string exp, RegularExpressionTable* regex_table) {
        std::vector<LanguageToken*> postfix = infixToPostfix(exp);
        NFA* result_nfa =  postfixToNFA(postfix, regex_table);
        for(int i = 0; i < (int)postfix.size(); i++) {
                delete postfix[i];
        }
        return result_nfa;
}

LanguageTokenType LanguageSpecParser::getTokenType(std::string operand) {
        if(operand.length() > 1) {
                return LanguageTokenType::EXPRESSION;
        }
        return LanguageTokenType::EXPRESSION;
}

std::vector<LanguageToken*> LanguageSpecParser::infixToPostfix(std::string exp) {
        std::vector<LanguageToken*> language_tokens;
        std::stack<LanguageToken*> st;
        bool concat = false;
        int pos = 0;
        while(pos < (int)exp.length()) {
                if(exp[pos] == '\\' && pos != (int)exp.length() - 1 && (isReservedSymbol(exp[pos + 1]) || exp[pos + 1] == 'L')) {
                        if(concat)
                                addOperator('&', language_tokens, st);
                        if(exp[pos + 1] == 'L')
                                language_tokens.push_back(new LanguageToken("", LanguageTokenType::CHARACTER));
                        else
                                language_tokens.push_back(new LanguageToken(exp.substr(pos + 1, 1), LanguageTokenType::CHARACTER));
                        pos++;
                        concat = true;
                } else if((!Util::isWhiteSpace(exp[pos]) && !isReservedSymbol(exp[pos]))&&
                          (pos == exp.length() - 1 || !(Util::isASCIIChar(exp[pos + 1]) || Util::isASCIIDigit(exp[pos + 1])))) {
                        if(concat)
                                addOperator('&', language_tokens, st);
                        language_tokens.push_back(new LanguageToken(exp.substr(pos, 1), LanguageTokenType::CHARACTER));
                        concat = true;
                } else if(Util::isASCIIChar(exp[pos])) {
                        if(concat)
                                addOperator('&', language_tokens, st);
                        int i;
                        for(i = 1; i + pos < exp.length()
                            && (Util::isASCIIChar(exp[pos + i]) || Util::isASCIIDigit(exp[pos + i])); i++) ;
                        std::string operand = exp.substr(pos, i);
                        language_tokens.push_back(new LanguageToken(operand, getTokenType(operand)));
                        pos += i - 1;
                        concat = true;
                } else if(exp[pos] == '(') {
                        if(concat)
                                addOperator('&', language_tokens, st);
                        st.push(new LanguageToken(LanguageTokenType::OPEN_BRACKET));
                        concat = false;
                } else if(exp[pos] == ')') {
                        while((!st.empty()) && st.top()->getType() != LanguageTokenType::OPEN_BRACKET) {
                                language_tokens.push_back(st.top());
                                st.pop();
                        }
                        if(st.empty() || st.top()->getType() != LanguageTokenType::OPEN_BRACKET) {
                                std::cerr << "Couldn't read the input file.\nProgram is exitting...";
                                exit(0);
                        } else {
                                st.pop();
                        }
                        concat = true;
                } else if(isValidOperator(exp[pos])) {
                        concat = exp[pos] == '*' || exp[pos] == '+';
                        addOperator(exp[pos], language_tokens, st);
                }
                pos++;
        }

        while(!st.empty()) {
                language_tokens.push_back(st.top());
                st.pop();
        }
        return language_tokens;
}

NFA* LanguageSpecParser::postfixToNFA(std::vector<LanguageToken*> language_tokens, RegularExpressionTable* regex_table) {
        for(int i = 0; i < (int)language_tokens.size(); i++) {
                // Convert values to NFA using Abdella functions
        }
}

void LanguageSpecParser::addOperator(char oper, vector<LanguageToken*> &language_tokens, stack<LanguageToken*> &st) {
        while((!st.empty()) && st.top()->getType() == LanguageTokenType::OPERATOR &&
              getOperatorPrecedence(oper) <= getOperatorPrecedence(st.top()->getValue()[0])) {
                language_tokens.push_back(st.top());
                st.pop();
        }
        string oper_str = " ";
        oper_str[0] = oper;
        st.push(new LanguageToken(oper_str, LanguageTokenType::OPERATOR));
        if(oper == '*' || oper == '+') {
                language_tokens.push_back(new LanguageToken(LanguageTokenType::NULL_TOKEN));
        }
}

bool LanguageSpecParser::isValidOperator(char oper) {
        return VALID_OPERATORS.find(oper) != VALID_OPERATORS.end();
}

int LanguageSpecParser::getOperatorPrecedence(char oper) {
        return OPERATOR_PRECEDENCE[oper];
}

bool LanguageSpecParser::isReservedSymbol(char sym) {
        return RESERVED_SYM.find(sym) != RESERVED_SYM.end();
}
