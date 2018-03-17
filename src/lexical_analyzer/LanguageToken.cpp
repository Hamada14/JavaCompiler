#include "lexical_analyzer/LanguageToken.hpp"

LanguageToken::LanguageToken(LanguageTokenType token_type): token_type(token_type){
}

LanguageToken::LanguageToken(std::string value, LanguageTokenType token_type): value(value), token_type(token_type) {
}

LanguageToken::LanguageToken(NFA* nfa): token_type(LanguageTokenType::EXPRESSION) {
        this->nfa = nfa;
}

LanguageToken::~LanguageToken() {
    delete nfa;
}

void LanguageToken::setNFA(NFA* nfa) {
    this->nfa = nfa;
}

NFA* LanguageToken::getNFA() { return nfa; }

bool LanguageToken::hasNFA() { return nfa == NULL; }

std::string LanguageToken::getValue() {
  return value;
}

LanguageTokenType LanguageToken::getType() {
  return token_type;
}
