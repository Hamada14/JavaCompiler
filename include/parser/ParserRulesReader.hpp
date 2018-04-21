#ifndef PARSERRULESREADER_HPP
#define PARSERRULESREADER_HPP

#include <fstream>
#include <vector>
#include <string>

#include "parser/ProductionRule.hpp"

class ParserRulesReader {
public:
  ParserRulesReader();
  ~ParserRulesReader();

  std::map<std::string, std::vector<ProductionRule> > getLL1Grammar(std::ifstream* input_file,
                                                                    std::ofstream* modified_rules);
private:
  const static std::string INVALID_INPUT_FILE_MESSAGE;

  std::map<std::string, std::vector<ProductionRule> > readRules(std::ifstream *input);
  std::vector<std::string> readFile(std::ifstream *input_file);
  std::map<std::string, std::vector<ProductionRule> > parseRules(std::set<std::string> rule_ids,
                                                                 std::vector<std::string> input);

  std::set<std::string> extractRuleIdentifiers(std::vector<std::string> &input);
  bool isRuleDefinitionLine(std::string &line);

  void printRulesTable(std::map<std::string,std::vector<ProductionRule> > rule_table, std::ostream& o_stream);
};
#endif
