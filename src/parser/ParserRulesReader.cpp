#include "parser/ParserRulesReader.hpp"

#include <iostream>
#include "Util.hpp"

const std::string ParserRulesReader::INVALID_INPUT_FILE_MESSAGE = "Parsing Rules input file is invalid.";

ParserRulesReader::ParserRulesReader() {
}

ParserRulesReader::~ParserRulesReader() {
}


std::map<std::string, std::vector<ProductionRule> > ParserRulesReader::getLL1Grammar(std::ifstream *input_file,
                                                                                     std::ofstream *modified_rules) {
  if (!input_file->is_open()) {
    std::cerr << INVALID_INPUT_FILE_MESSAGE << std::endl;
    exit(0);
  }
  std::map<std::string, std::vector<ProductionRule> > rule_table = readRules(input_file);
  
  printRulesTable(rule_table, *modified_rules);
  return rule_table;
}

std::map<std::string, std::vector<ProductionRule> > ParserRulesReader::readRules(std::ifstream *input) {
  std::vector<std::string> file_lines = readFile(input);
  std::set<std::string> rules_identifier = extractRuleIdentifiers(file_lines);
  return parseRules(rules_identifier, file_lines);
}

std::map<std::string, std::vector<ProductionRule> > ParserRulesReader::parseRules(std::set<std::string> rule_ids,
                                                                                  std::vector<std::string> input) {
  std::map<std::string, std::vector<ProductionRule> > rules;
  std::string current_id;
  for(int cur_line = 0; (size_t)cur_line < input.size();) {
    current_id = ProductionRule::getIdentifier(input[cur_line]);
    std::string full_def = input[cur_line].substr(input[cur_line].find(ProductionRule::RULE_DEFINITION_OPERATOR) + 1);
    int next_line = cur_line + 1;
    while((size_t)next_line < input.size() && !ProductionRule::isRuleDefinition(input[next_line])) {
      full_def += input[next_line];
      next_line++;
    }
    std::vector<std::string> raw_rules = Util::splitQuoteSensitive(full_def, ProductionRule::RULES_SEPARATOR);
    for(std::string raw_rule : raw_rules) {
      if(Util::isEmptyString(raw_rule)) {
        continue;
      }
      ProductionRule rule(raw_rule, rule_ids);
      if(rule.isInvalid()) {
        std::cerr << "Invalid Rule specified in the declaration of {" << current_id << "}->{" << raw_rule << "}" << std::endl;
        exit(-1);
      }
      rules[current_id].push_back(rule);
    }
    cur_line = next_line;
  }
  return rules;
}

std::vector<std::string> ParserRulesReader::readFile(std::ifstream *input_file) {
  std::vector<std::string> file_lines;
  for (std::string line; getline(*input_file, line);) {
    file_lines.push_back(line);
  }
  return file_lines;
}

std::set<std::string> ParserRulesReader::extractRuleIdentifiers(std::vector<std::string>&input) {
  std::set<std::string> identifiers;
  for(std::string line : input) {
    if(ProductionRule::isRuleDefinition(line)) {
      identifiers.insert(ProductionRule::getIdentifier(line));
    }
  }
  return identifiers;
}

void ParserRulesReader::printRulesTable(std::map<std::string, std::vector<ProductionRule> > rule_table,
                                        std::ostream& o_stream) {
    std::map<std::string, std::vector<ProductionRule> >::iterator it;
    for(it = rule_table.begin(); it != rule_table.end(); it++) {
      o_stream << ProductionRule::RULE_DEFINITION_START_MARKER << it->first << Util::WHITE_SPACE;
      o_stream << ProductionRule::RULE_DEFINITION_OPERATOR << Util::WHITE_SPACE;
      std::vector<ProductionRule> rules = it->second;
      for(size_t i = 0; i < rules.size(); i++) {
        std::vector<RuleToken> tokens = rules[i].getTokens();
        for(RuleToken token : tokens) {
          o_stream << Util::WHITE_SPACE;
          if(token.getType() == RuleTokenType::TERMINAL) {
            o_stream << Util::SINGLE_QUOTE << token.getValue()  << Util::SINGLE_QUOTE;
          } else {
            o_stream << token.getValue();
          }
        }
        if(i != rules.size() - 1) {
          o_stream << Util::WHITE_SPACE << ProductionRule::RULES_SEPARATOR;
        }
      }
      o_stream << std::endl;
    }
}
