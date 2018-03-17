#include "lexical_analyzer/LanguageSpecParser.hpp"


std::map<char, int> LanguageSpecParser::OPERATOR_PRECEDENCE =
{ {'-', 5}, {'*', 4}, {'+', 3}, {'&', 2}, {'|', 1} };
const std::set<char> LanguageSpecParser::VALID_OPERATORS = {'-', '*', '+', '|'};
const std::set<char> LanguageSpecParser::UNARY_OPERATORS = {'*', '+'};
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
        return LanguageTokenType::CHARACTER;
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
                                language_tokens.push_back(new LanguageToken("", LanguageTokenType::NULL_CHARACTER));
                        else
                                language_tokens.push_back(new LanguageToken(exp.substr(pos + 1, 1), LanguageTokenType::CHARACTER));
                        pos++;
                        concat = true;
                } else if((!Util::isWhiteSpace(exp[pos]) && !isReservedSymbol(exp[pos]))&&
                          (pos == (int)exp.length() - 1 || !(Util::isASCIIChar(exp[pos + 1]) || Util::isASCIIDigit(exp[pos + 1])))) {
                        if(concat)
                                addOperator('&', language_tokens, st);
                        language_tokens.push_back(new LanguageToken(exp.substr(pos, 1), LanguageTokenType::CHARACTER));
                        concat = true;
                } else if(Util::isASCIIChar(exp[pos])) {
                        if(concat)
                                addOperator('&', language_tokens, st);
                        int i;
                        for(i = 1; i + pos < (int)exp.length()
                            && (Util::isASCIIChar(exp[pos + i]) || Util::isASCIIDigit(exp[pos + i])); i++) ;
                        std::string operand = exp.substr(pos, i);
                        language_tokens.push_back(new LanguageToken(operand, LanguageTokenType::EXPRESSION));
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
                        concat = isUnaryOperator(exp[pos]);
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

void LanguageSpecParser::addOperator(char oper, vector<LanguageToken*> &language_tokens, stack<LanguageToken*> &st) {
        while((!st.empty()) && st.top()->getType() == LanguageTokenType::OPERATOR &&
              getOperatorPrecedence(oper) <= getOperatorPrecedence(st.top()->getValue()[0])) {
                language_tokens.push_back(st.top());
                st.pop();
        }
        string oper_str = " ";
        oper_str[0] = oper;
        st.push(new LanguageToken(oper_str, LanguageTokenType::OPERATOR));
        if(isUnaryOperator(oper)) {
                language_tokens.push_back(new LanguageToken(LanguageTokenType::NULL_TOKEN));
        }
}


NFA* LanguageSpecParser::postfixToNFA(std::vector<LanguageToken*> language_tokens, RegularExpressionTable* regex_table) {
        std::stack<LanguageToken*> st;
        for(LanguageToken* cur_token : language_tokens) {
                if(cur_token->getType() != LanguageTokenType::OPERATOR) {
                    st.push(cur_token);
                } else {
                    if(st.size() < 2) {
                        //Error handling routine
                    }
                    LanguageToken* op1 = st.top();
                    st.pop();
                    LanguageToken* op2 = st.top();
                    st.pop();
                    correctStackToken(op1, regex_table);
                    correctStackToken(op2, regex_table);
                    NFA* result;
                    switch(cur_token->getValue()[0]) {
                        case '*':
                            result = op1->getNFA()->asteriskOperation();
                            break;
                        case '-':
                            result = plusOperation(op1, op2);
                            break;
                        case '|':
                            result = op1->getNFA()->orOperation(*op2->getNFA());
                            break;
                        case '&':
                            result = op1->getNFA()->concatenateOperation(*op2->getNFA());
                            break;
                        case '+':
                            result = op1->getNFA()->plusOperation();
                            break;
                        default:
                            exit(-1);
                    }
                    st.push(new LanguageToken(result));
                    delete op1;
                    delete op2;
                }
        }
        return st.top()->getNFA();
}

void LanguageSpecParser::correctStackToken(LanguageToken* t1, RegularExpressionTable* regex_table) {
    if(t1->getType() == LanguageTokenType::EXPRESSION && !t1->hasNFA()) {
        if(!regex_table->hasExpression(t1->getValue())) {
            std::cout << " Error" << std::endl;
            exit(-1);
        }
        t1->setNFA(regex_table->getExpressionNFA(t1->getValue()));
    }
}

NFA* LanguageSpecParser::plusOperation(LanguageToken* t1, LanguageToken* t2) {
    char range_start = t1->getValue()[0];
    char range_end = t2->getValue()[0];
    NFA* accumulated_NFA = new NFA(range_start);
    if(!isValidRegexRange(range_start, range_end)) {
        std::cout << "Invalid Rege range" << std::endl;;
        exit(-1);
    }
    for(int i = range_start + 1; i <= range_end; i++) {
        NFA* char_NFA = new NFA(i);
        accumulated_NFA = accumulated_NFA->orOperation(*char_NFA);
        delete char_NFA;
    }
    return accumulated_NFA;
}

bool LanguageSpecParser::isValidRegexRange(char range_start, char range_end) {
        return (range_start == 'a' && range_end == 'z') || (range_start == 'A' && range_end == 'Z')
                || (range_start == '0' && range_end == '9');
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

bool LanguageSpecParser::isUnaryOperator(char oper) {
        return UNARY_OPERATORS.find(oper) != UNARY_OPERATORS.end();
}
