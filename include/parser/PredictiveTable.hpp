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

#include "..\..\include\parser/RuleToken.hpp"
#include "..\..\include\parser/ProductionRule.hpp"
#include "..\..\include\Constants.hpp"
#include "..\..\include\parser/RuleToken.hpp"

enum class TRANSITIONSTATE {SYNCH, ERROR, LEGAL};

class PredictiveTable {
private:
    enum class TYPE {FIRST, FOLLOW};
    std::map<std::string, std::vector<ProductionRule> > &ll1_grammar;
    std::map<std::string, std::unordered_set<std::string> > first, follow;
    std::map<std::string, std::map<std:: string, ProductionRule> > table;
    std::string start_state;

    std::unordered_set<std::string> getFirst(std::string state);
    std::unordered_set<std::string> getFollow(std::string state);
    void calcRHSFollow(std::unordered_set<std::string> &cur, std::string &state);
    bool checkTerminals(RuleToken &r, std::unordered_set<std::string> &cur, std::string &state, TYPE type);

public:
    PredictiveTable(std::map<std::string, std::vector<ProductionRule> > &ll1_grammar, std::string start_state);
    std::vector<RuleToken> getTransition(std::string, std::string);
    TRANSITIONSTATE getTransitionType(std::string, std::string);
};



#endif /* PredictiveTable_hpp */
