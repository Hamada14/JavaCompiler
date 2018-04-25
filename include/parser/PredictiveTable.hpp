//  PredictiveTable.hpp
//
//  Created by Ahmed on 4/25/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#ifndef PredictiveTable_hpp
#define PredictiveTable_hpp

#include <stdio.h>

#include <vector>
#include <string>
#include <map>
#include <unordered_set>

#include "parser/RuleToken.hpp"
#include "parser/ProductionRule.hpp"
#include "Constants.hpp"

class PredictiveTable {
private:
    enum class TYPE {FIRST, FOLLOW};
    std::map<std::string, std::vector<ProductionRule> > &ll1_grammar;
    std::map<std::string, std::unordered_set<std::string> > first, follow;
    std::map<std::string, std::map<std:: string, ProductionRule> > table;
    
    std::unordered_set<std::string> getFirst(std::string &state);
    std::unordered_set<std::string> getFollow(std::string &state);
    void calcRHSFollow(std::unordered_set<std::string> &cur, std::string &state);
    bool checkTerminals(RuleToken &r, std::unordered_set<std::string> &cur, std::string &state, TYPE type);
    
public:
    PredictiveTable(std::map<std::string, std::vector<ProductionRule> > &ll1_grammar);
    std::vector<RuleToken> getTransition(std::string, std::string);
};



#endif /* PredictiveTable_hpp */
