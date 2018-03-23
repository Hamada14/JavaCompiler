#ifndef LanguageTokens_hpp
#define LanguageTokens_hpp

#include "NFA.hpp"
#include <string>

enum class LanguageTokenType {OPERATOR, EXPRESSION, CHARACTER, NULL_TOKEN, OPEN_BRACKET, CLOSED_BRACKET, NULL_CHARACTER};

class LanguageToken {
public:
    LanguageToken(LanguageTokenType);
    LanguageToken(std::string, LanguageTokenType);
    LanguageToken(NFA*, bool);
    ~LanguageToken();

    std::string getValue();
    LanguageTokenType getType();
    
    bool hasNFA();
    NFA* getNFA();
    void setNFA(NFA*, bool);

private:
    NFA* nfa;
    std::string value;
    LanguageTokenType token_type;
    bool delete_nfa;
};
#endif
