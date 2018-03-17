#ifndef LanguageTokens_hpp
#define LanguageTokens_hpp

#include <string>
#include "NFA.hpp"

enum class LanguageTokenType {OPERATOR, EXPRESSION, CHARACTER, NULL_TOKEN, OPEN_BRACKET, CLOSED_BRACKET, NULL_CHARACTER};

class LanguageToken {
  public:
    LanguageToken(LanguageTokenType);
    LanguageToken(std::string, LanguageTokenType);
    LanguageToken(NFA*);
    ~LanguageToken();

    std::string getValue();
    LanguageTokenType getType();
    bool hasNFA();
    NFA* getNFA();
    void setNFA(NFA*);

  private:
    NFA* nfa;
    std::string value;
    LanguageTokenType token_type;
};
#endif
