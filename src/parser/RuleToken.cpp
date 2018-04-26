#include "..\..\include\parser/RuleToken.hpp"

#include <iostream>

#include "..\..\include\Constants.hpp"

const std::regex RuleToken::TERMINAL_TOKEN_REGEX("\\\'.+\\\'");
const std::regex RuleToken::NON_TERMINAL_TOKEN_REGEX("[a-zA-Z][a-zA-Z0-9_]*");

const std::set<char> RuleToken::RESERVED_SYMBOLS = { '\'', '|', '=', '\\'};

const char RuleToken::BACK_SLASH = '\\';

RuleToken::RuleToken(std::string value) {
    this->is_invalid = false;
    if (value == Constants::LAMBDA) {
        this->type = RuleTokenType::LAMBDA_TERMINAL;
        this->value = value;
    } else if(value == Constants::END_OF_INPUT) {
        this->type = RuleTokenType::END_OF_INPUT;
        this->value = value;
    } else if (regex_match(value, TERMINAL_TOKEN_REGEX)) {
        this->type = RuleTokenType::TERMINAL;
        this->value = value.substr(1, value.length() - 2);
        if(!checkEscapeCharacters()) {
            this->is_invalid = true;
            std::cerr << "Invalid usage of reserved symbols." << std::endl;
        }
    } else if (regex_match(value, NON_TERMINAL_TOKEN_REGEX)) {
        this->type = RuleTokenType::NON_TERMINAL;
        this->value = value;
    } else {
        this->is_invalid = true;
    }
}

RuleToken::~RuleToken() {
}

bool RuleToken::operator== (const RuleToken& token) const {
    return this->getType() == token.getType() && this->getValue() == token.getValue();
}

bool RuleToken::operator != (const RuleToken& token) const {
    return this->getType() != token.getType() || this->getValue() != token.getValue();
}

bool RuleToken::operator< (const RuleToken& token) const {
    if(this->getType() != token.getType()) {
        return this->getType() < token.getType();
    }
    return this->getValue() < token.getValue();
}

RuleTokenType RuleToken::getType() const {
    return this->type;
}

std::string RuleToken::getValue() const {
    return this->value;
}

bool RuleToken::isInvalid() {
    return this->is_invalid;
}

bool RuleToken::checkEscapeCharacters() {
    for(size_t index = 0; index < this->value.length(); index++) {
        char current_char = this->value[index];
        bool has_next_char = index != this->value.length() - 1;
        char next_char = has_next_char ? this->value[index + 1] : '\0';
        if(has_next_char && current_char == BACK_SLASH && isReservedSymbol(next_char)) {
            index++;
        } else if(isReservedSymbol(current_char) || current_char == BACK_SLASH) {
            return false;
        }
    }
    return true;
}

bool RuleToken::isReservedSymbol(char symbol) {
    return RESERVED_SYMBOLS.find(symbol) != RESERVED_SYMBOLS.end();
}
