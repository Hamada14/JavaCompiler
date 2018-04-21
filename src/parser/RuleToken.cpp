#include "parser/RuleToken.hpp"

const std::string RuleToken::LAMBDA_VALUE = "\\L";

const std::regex RuleToken::TERMINAL_TOKEN_REGEX("\\\'.+\\\'");
const std::regex RuleToken::NON_TERMINAL_TOKEN_REGEX("[a-zA-Z][a-zA-Z0-9_]*");
#include <iostream>
RuleToken::RuleToken(std::string value) {
    this->is_invalid = false;
    if (value == LAMBDA_VALUE) {
        this->type = RuleTokenType::LAMBDA_TERMINAL;
        this->value = value;
    } else if (regex_match(value, TERMINAL_TOKEN_REGEX)) {
        this->type = RuleTokenType::TERMINAL;
        this->value = value.substr(1, value.length() - 2);
    } else if (regex_match(value, NON_TERMINAL_TOKEN_REGEX)) {
        this->type = RuleTokenType::NON_TERMINAL;
        this->value = value;
    } else {
        this->is_invalid = true;
    }
}

RuleToken::~RuleToken() {
}

RuleTokenType RuleToken::getType() {
    return this->type;
}

std::string RuleToken::getValue() {
    return this->value;
}

bool RuleToken::isInvalid() {
    return this->is_invalid;
}
