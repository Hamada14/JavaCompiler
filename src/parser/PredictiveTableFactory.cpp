#include "parser/PredictiveTableFactory.hpp"

PredictiveTableFactory* PredictiveTableFactory::instance = NULL;

PredictiveTableFactory::PredictiveTableFactory(ParserRulesReader* reader) : reader(reader) {}

PredictiveTableFactory::~PredictiveTableFactory() {
  delete reader;
};

PredictiveTableFactory* PredictiveTableFactory::getInstance() {
  if(instance == NULL) {
    instance = new PredictiveTableFactory(new ParserRulesReader());
  }
  return instance;
}

PredictiveTable PredictiveTableFactory::getTable(std::ifstream *input_file, std::ofstream *fixed_rules_output) {
  std::map<std::string, std::vector<ProductionRule> > ll1_grammar = reader->getLL1Grammar(input_file, fixed_rules_output);

  PredictiveTable table;
  return table;
}
