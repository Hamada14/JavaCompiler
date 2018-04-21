#include "parser/ProductionRule.hpp"

#include <cassert>
#include <iostream>

#include "Util.hpp"

const char ProductionRule::RULE_DEFINITION_START_MARKER = '#';
const char ProductionRule::RULE_DEFINITION_OPERATOR = '=';
const char ProductionRule::RULES_SEPARATOR = '|';
const std::regex ProductionRule::RULE_DEFINITION_REGEX("#\\s*[a-zA-Z][a-zA-Z0-9_]+\\s*=.*");

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

ProductionRule::~ProductionRule() {}

std::vector<std::string> ProductionRule::tokenize(std::string raw_rule) {
  std::vector<std::string> result;
  for(std::size_t pos = 0; pos < raw_rule.length(); pos++) {
    if(Util::isWhiteSpace(raw_rule[pos])) {
      continue;
    } else if(raw_rule[pos] == '\'') {
      size_t end_pos = raw_rule.find('\'', pos + 1);
      if(end_pos == std::string::npos) {
        this->is_invalid = true;
        end_pos = raw_rule.length() - 1;
      }
      result.push_back(raw_rule.substr(pos, end_pos - pos + 1));
      pos = end_pos;
    } else if(Util::isASCIIChar(raw_rule[pos])) {
      size_t end_pos = pos;
      while(end_pos < raw_rule.length() &&
          (Util::isASCIIChar(raw_rule[end_pos]) || Util::isASCIIDigit(raw_rule[end_pos]) || raw_rule[end_pos] == '_')) {
        end_pos++;
      }
      result.push_back(raw_rule.substr(pos, end_pos - pos));
      pos = end_pos - 1;
    } else {
      this->is_invalid = true;
      break;
    }
  }

  return result;
}

std::vector<RuleToken> ProductionRule::getTokens() {
  return tokens;
}

bool ProductionRule::isInvalid() {
  return this->is_invalid;
}

bool ProductionRule::isRuleDefinition(std::string& line) {
  return regex_match(line, RULE_DEFINITION_REGEX);
}

std::string ProductionRule::getIdentifier(std::string& line) {
  std::size_t definition_start_pos = line.find(RULE_DEFINITION_START_MARKER) + 1;
  std::size_t definition_end_pos = line.find(RULE_DEFINITION_OPERATOR) - 1;
  assert(definition_start_pos != std::string::npos);
  assert(definition_end_pos != std::string::npos);
  std::string id = line.substr(definition_start_pos, definition_end_pos - definition_start_pos + 1);
  return Util::trim(id);
}
