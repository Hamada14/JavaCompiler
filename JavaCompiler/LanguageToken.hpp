#ifndef LanguageTokens_hpp
#define LanguageTokens_hpp

#include <string>

enum class LanguageTokenType {OPERATOR, EXPRESSION, CHARACTER, NULL_TOKEN, OPEN_BRACKET, CLOSED_BRACKET};

class LanguageToken {
  public:
    LanguageToken(LanguageTokenType);
    LanguageToken(std::string, LanguageTokenType);

    std::string getValue();
    LanguageTokenType getType();
  private:
    std::string value;
    LanguageTokenType token_type;
};
#endif
