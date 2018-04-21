#ifndef PREDICTIVETABLE_HPP_INCLUDED
#define PREDICTIVETABLE_HPP_INCLUDED

#include <vector>
#include <string>

#include "parser/RuleToken.hpp"

class PredictiveTable {
public:
  /*
  * Parameters => Current state and input token.
  */
  std::vector<RuleToken> getTransition(std::string, std::string);
};

#endif
