#include "LanguageToken.hpp"

LanguageToken::LanguageToken(LanguageTokenType token_type): token_type(token_type){
}

LanguageToken::LanguageToken(std::string value, LanguageTokenType token_type): value(value), token_type(token_type) {
}

std::string LanguageToken::getValue() {
  return value;
}

LanguageTokenType LanguageToken::getType() {
  return token_type;
}
