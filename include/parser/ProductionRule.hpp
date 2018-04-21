#ifndef PRODUCTIONRULE_HPP
#define PRODUCTIONRULE_HPP

#include <vector>
#include <set>
#include <string>

#include "parser/RuleToken.hpp"

class ProductionRule {
public:
  const static char RULE_DEFINITION_START_MARKER;
  const static char RULE_DEFINITION_OPERATOR;
  const static char RULES_SEPARATOR;
  const static std::regex RULE_DEFINITION_REGEX;


  ProductionRule(std::string raw_rule, std::set<std::string> valid_ids);
  ~ProductionRule();

  static bool isRuleDefinition(std::string& line);
  static std::string getIdentifier(std::string& line);

  std::vector<RuleToken> getTokens();
  bool isInvalid();

private:
  std::vector<RuleToken> tokens;
  bool is_invalid;

  std::vector<std::string> tokenize(std::string raw_rule);
};
#endif // ifndef PRODUCTIONRULE_HPP
