#ifndef PREDICTIVETABLEBUILDER_HPP_INCLUDED
#define PREDICTIVETABLEBUILDER_HPP_INCLUDED

#include <fstream>

#include "parser/PredictiveTable.hpp"

class PredictiveTableBuilder {
public:
  PredictiveTableBuilder();
  PredictiveTable buildTable(std::ifstream*);
};

#endif
