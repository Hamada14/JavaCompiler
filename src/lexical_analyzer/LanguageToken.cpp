#include "lexical_analyzer/LanguageToken.hpp"

LanguageToken::LanguageToken(LanguageTokenType token_type) : token_type(token_type){
        this->nfa = NULL;
}

LanguageToken::LanguageToken(std::string value, LanguageTokenType token_type) : value(value), token_type(token_type) {
        this->nfa = NULL;
}

LanguageToken::LanguageToken(NFA* nfa, bool) : token_type(LanguageTokenType::EXPRESSION) {
        this->nfa = nfa;
}

LanguageToken::~LanguageToken() {
        if(hasNFA() && delete_nfa)
                delete nfa;
}

void LanguageToken::setNFA(NFA* nfa, bool delete_nfa) {
        this->nfa = nfa;
        this->delete_nfa = delete_nfa;
}

NFA* LanguageToken::getNFA() {
        return nfa;
}

bool LanguageToken::hasNFA() {
        return nfa != NULL;
}

std::string LanguageToken::getValue() {
        return value;
}

LanguageTokenType LanguageToken::getType() {
        return token_type;
}
