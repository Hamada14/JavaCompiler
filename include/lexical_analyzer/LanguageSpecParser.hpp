#ifndef LanguageSpecParser_hpp
#define LanguageSpecParser_hpp

#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

#include "LanguageToken.hpp"
#include "NFA.hpp"
#include "RegularExpressionTable.hpp"
#include "..\..\include\Util.hpp"

class LanguageSpecParser {
public:
    NFA* parseRegularExpression(std::string, RegularExpressionTable*);

    bool isValidOperator(char);
    bool isReservedSymbol(char);
    bool isUnaryOperator(char);
    bool isValidRegexRange(char, char);

private:
    static std::map<char, int> OPERATOR_PRECEDENCE;
    static const std::set<char> RESERVED_SYM;
    static const std::set<char> VALID_OPERATORS;
    static const std::set<char> UNARY_OPERATORS;

    std::vector<LanguageToken*> infixToPostfix(std::string);
    NFA* postfixToNFA(std::vector<LanguageToken*>, RegularExpressionTable*);

    int getOperatorPrecedence(char);
    void correctStackToken(LanguageToken*, RegularExpressionTable*);

    LanguageTokenType getTokenType(std::string);

    void addOperator(char, vector<LanguageToken*> &, stack<LanguageToken*> &);

    NFA* rangeOperation(LanguageToken*, LanguageToken*);
};
#endif
