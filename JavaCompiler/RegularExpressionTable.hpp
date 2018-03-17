#ifndef RegexExpressionTable_hpp
#define RegexExpressionTable_hpp

#include <string>
#include "NFA.hpp"
#include <map>

class RegularExpressionTable {
  public:
      bool hasExpression(std::string);
      NFA* getExpressionNFA(std::string);
  private:
      std::map<std::string, NFA*> nfa_map;
};
#endif
