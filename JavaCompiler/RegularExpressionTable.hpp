#ifndef RegexExpressionTable_hpp
#define RegexExpressionTable_hpp

#include <string>
#include "NFA.hpp"
#include <map>

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
