#ifndef PREDICTIVETABLE_HPP_INCLUDED
#define PREDICTIVETABLE_HPP_INCLUDED

#include <vector>

class PredictiveTable {
public:
  /*
  * Parameters => Current state and input token.
  */
  std::vector<Token> getTransition(std::string, std::string);
};

#endif
