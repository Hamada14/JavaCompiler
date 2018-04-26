#ifndef PARSERRULESREADER_HPP
#define PARSERRULESREADER_HPP

#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "parser/ProductionRule.hpp"
#include "Constants.hpp"

struct LL1Grammar {
    std::map<std::string, std::vector<ProductionRule> > rule_table;
    std::string start_state;

    LL1Grammar(std::map<std::string, std::vector<ProductionRule> > rule_table,
               std::string start_state) : rule_table(rule_table), start_state(start_state) {};
};

class ParserRulesReader {
public:
  ParserRulesReader();
  ~ParserRulesReader();

  LL1Grammar getLL1Grammar(std::ifstream* input_file, std::ofstream* modified_rules);

private:
  const static std::string INVALID_INPUT_FILE_MESSAGE;

  int static generated_state_id;

  static std::string createStateName(int id);
  static std::string generateState();

  LL1Grammar readRules(std::ifstream *input);
  std::vector<std::string> readFile(std::ifstream *input_file);
  LL1Grammar parseRules(std::set<std::string> rule_ids, std::vector<std::string> input);

  void eliminateLeftRecursion(std::map<std::string, std::vector<ProductionRule> >& rule_table);
  void leftFactorGrammar(std::map<std::string, std::vector<ProductionRule> >& rule_table);
  void leftFactorProduction(std::string rule_name, std::map<std::string, std::vector<ProductionRule> >& rule_table);

  std::vector<ProductionRule> leftImmedEliminate(std::vector<ProductionRule>& productions,
                                                 std::string state_name, int new_state);

  std::set<std::string> extractRuleIdentifiers(std::vector<std::string> &input);
  bool isRuleDefinitionLine(std::string &line);

  void printRulesTable(std::map<std::string,std::vector<ProductionRule> > rule_table, std::ostream& o_stream);
};
#endif
