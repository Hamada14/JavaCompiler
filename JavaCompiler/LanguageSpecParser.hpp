#ifndef LanguageSpecParser_hpp
#define LanguageSpecParser_hpp

#include "LanguageToken.hpp"
#include "NFA.hpp"
#include "RegularExpressionTable.hpp"
#include "Util.hpp"
#include <iostream>
#include <map>
#include <set>
#include <stack>
#include <vector>

class LanguageSpecParser {
  public:
    NFA* parseRegularExpression(std::string, RegularExpressionTable*);

  private:
    std::vector<LanguageToken*> infixToPostfix(std::string);
    NFA* postfixToNFA(std::vector<LanguageToken*>, RegularExpressionTable*);

    bool isValidOperator(char);
    bool isReservedSymbol(char);
    int getOperatorPrecedence(char);

    LanguageTokenType getTokenType(std::string);

    void addOperator(char, vector<LanguageToken*> &, stack<LanguageToken*> &);

    static std::map<char, int> OPERATOR_PRECEDENCE;
    static const std::set<char> RESERVED_SYM;
    static const std::set<char> VALID_OPERATORS;
};
#endif
