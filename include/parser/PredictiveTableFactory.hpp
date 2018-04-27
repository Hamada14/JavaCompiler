#ifndef PREDICTIVETABLEFACTORY_HPP_INCLUDED
#define PREDICTIVETABLEFACTORY_HPP_INCLUDED

#include <fstream>

#include "parser/PredictiveTable.hpp"
#include "parser/ParserRulesReader.hpp"

class PredictiveTableFactory {
public:
  ~PredictiveTableFactory();
  static PredictiveTableFactory* getInstance();

  PredictiveTable* getTable(std::ifstream *input_file, std::ofstream *fixed_rules_output);

private:
  static PredictiveTableFactory* instance;
  ParserRulesReader* reader;

  PredictiveTableFactory(ParserRulesReader* reader);

};

#endif // ifndef PREDICTIVETABLEFACTORYHPP_INCLUDED
