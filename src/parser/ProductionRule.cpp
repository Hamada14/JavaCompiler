#include "parser/ProductionRule.hpp"

#include <cassert>
#include <iostream>
#include <limits>

#include "Util.hpp"
#include "Constants.hpp"

const char ProductionRule::RULE_DEFINITION_START_MARKER = '#';
const char ProductionRule::RULE_DEFINITION_OPERATOR = '=';
const char ProductionRule::RULES_SEPARATOR = '|';
const std::regex ProductionRule::RULE_DEFINITION_REGEX("#\\s*[a-zA-Z][a-zA-Z0-9_]*\\s*=.*");

ProductionRule::ProductionRule(std::vector<RuleToken> tokens) : tokens(tokens) {
}

ProductionRule::ProductionRule(std::string raw_rule, std::set<std::string> valid_ids) {
    this->is_invalid = false;
    std::vector<std::string> raw_tokens = tokenize(raw_rule);
    for(std::string raw_token : raw_tokens) {
        RuleToken token(raw_token);
        if(token.getType() == RuleTokenType::NON_TERMINAL && valid_ids.find(token.getValue()) == valid_ids.end()) {
            std::cerr << "Undeclared token -> {" << token.getValue() << "}" << std::endl;
            this->is_invalid = true;
        }
        if(token.isInvalid()) {
            this->is_invalid = true;
        }
        tokens.push_back(token);
    }
}

ProductionRule::~ProductionRule() {
}

std::vector<std::string> ProductionRule::tokenize(std::string raw_rule) {
    std::vector<std::string> result;
    for(std::size_t pos = 0; pos < raw_rule.length(); pos++) {
        if(Util::isWhiteSpace(raw_rule[pos])) {
            continue;
        } else if(raw_rule[pos] == Util::SINGLE_QUOTE) {
            size_t end_pos;
            for(end_pos = pos + 1; end_pos < raw_rule.length(); end_pos++) {
                if(raw_rule[end_pos] == RuleToken::BACK_SLASH) {
                    end_pos++;
                } else if(raw_rule[end_pos] == Util::SINGLE_QUOTE) {
                    break;
                } else if(end_pos == raw_rule.length() - 1) {
                    end_pos = std::string::npos;
                }
            }
            if(end_pos == std::string::npos) {
                this->is_invalid = true;
                end_pos = raw_rule.length() - 1;
            }
            result.push_back(raw_rule.substr(pos, end_pos - pos + 1));
            pos = end_pos;
        } else if(Util::isASCIIChar(raw_rule[pos])) {
            size_t end_pos = pos;
            while(end_pos < raw_rule.length() &&
                  (Util::isASCIIChar(raw_rule[end_pos]) || Util::isASCIIDigit(raw_rule[end_pos]) || raw_rule[end_pos] ==
                   '_')) {
                end_pos++;
            }
            result.push_back(raw_rule.substr(pos, end_pos - pos));
            pos = end_pos - 1;
        } else if(pos < raw_rule.size() - 1 && Constants::LAMBDA == raw_rule.substr(pos, pos + 2)) {
            result.push_back(Constants::LAMBDA);
            pos++;
        } else {
            this->is_invalid = true;
            break;
        }
    }

    return result;
}

bool ProductionRule::canLeftImmedSubstitute(std::string non_terminal_state) {
    return tokens.size() != 0 && tokens[0].getType() == RuleTokenType::NON_TERMINAL
           && tokens[0].getValue() == non_terminal_state;
}

std::vector<ProductionRule> ProductionRule::leftImmedSubstitute(std::vector<ProductionRule> rules) {
    std::vector<ProductionRule> result;
    std::vector<RuleToken> appended_tokens = this->tokens;
    appended_tokens.erase(appended_tokens.begin());
    for(ProductionRule rule : rules) {
        std::vector<RuleToken> tokens = rule.getTokens();
        tokens.insert(std::end(tokens), std::begin(appended_tokens), std::end(appended_tokens));
        result.push_back(ProductionRule(tokens));
    }
    return result;
}

std::vector<RuleToken> ProductionRule::getTokens() {
    return tokens;
}

std::vector<RuleToken> ProductionRule::getPrefixTokens(size_t token_count) {
    return std::vector<RuleToken>(tokens.begin(), tokens.begin() + token_count);
}

void ProductionRule::popTokens(size_t token_count) {
    tokens.erase(tokens.begin(), tokens.begin() + token_count);
}

RuleToken ProductionRule::getToken(size_t index) {
    return tokens[index];
}

size_t ProductionRule::getTokenCount() {
    return tokens.size();
}

bool ProductionRule::isInvalid() {
    return this->is_invalid;
}

bool ProductionRule::isRuleDefinition(std::string& line) {
    return regex_match(line, RULE_DEFINITION_REGEX);
}

std::string ProductionRule::getIdentifier(std::string& line) {
    std::size_t definition_start_pos = line.find(RULE_DEFINITION_START_MARKER);
    std::size_t definition_end_pos = line.find(RULE_DEFINITION_OPERATOR);
    assert(definition_start_pos != std::string::npos);
    assert(definition_end_pos != std::string::npos);
    std::string id = line.substr(definition_start_pos + 1, definition_end_pos - definition_start_pos - 1);
    return Util::trim(id);
}

size_t ProductionRule::getCommonPrefixTokenCount(std::vector<ProductionRule> rules) {
    size_t max_len = std::numeric_limits<size_t>::max();
    for(ProductionRule rule : rules) {
        max_len = std::min(max_len, rule.getTokenCount());
    }
    for(size_t prefix_index = 0; prefix_index < max_len; prefix_index++) {
        RuleToken current_token = rules[0].getToken(prefix_index);
        for(ProductionRule rule : rules) {
            if(rule.getToken(prefix_index) != current_token) {
                return prefix_index;
            }
        }
    }
    return max_len;
}
