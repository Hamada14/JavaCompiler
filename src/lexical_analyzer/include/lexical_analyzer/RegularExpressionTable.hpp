#ifndef RegexExpressionTable_hpp
#define RegexExpressionTable_hpp

#include "NFA.hpp"
#include <map>
#include <string>

class RegularExpressionTable {
  public:
      ~RegularExpressionTable();

      bool hasExpression(std::string);
      void addExpressionNFA(std::string, NFA*);
      NFA* getExpressionNFA(std::string);
      std::vector<NFA*> getValues();
      void clear();
  private:
      std::map<std::string, NFA*> nfa_map;
};
#endif
