#ifndef PARSETREECREATOR_HPP_INCLUDED
#define PARSETREECREATOR_HPP_INCLUDED

#include <fstream>

#include "lexical_analyzer/Tokenizer.hpp"
#include "parser/PredictiveTable.hpp"

class ParseTreeCreator {
public:
  ParseTreeCreator(Tokenizer, PredictiveTable);
  createTable(std::ifstream*, std::ofstream*);
};

#endif
