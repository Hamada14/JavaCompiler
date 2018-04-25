#include "parser/PrdictiveTable.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>



PredictiveTable(std::map<std::string, std::vector<ProductionRule> > * ll1_grammar) {
    this->ll1_grammar = ll1_grammar;
    for(auto &it: ll1_grammar)
        calcFirst(it.f), calcFollow(it.f);
    
}

std::unordered_set<std::string> PredictiveTable:: calcFirst(std:: string &state) {
    
    if(!ll1_grammar[state].count(state))
        cerr<< "Undefined token\n", exit(0);
    
    if(first.count(state)) return first[state];
    
    std:: unordered_set<std::string> &cur_first = first[state];
    for(ProductionRule &pr: ll1_grammar[state]) {
        vector<RuleToken> &tokens = pr.getTokens();
        for(RuleToken &r : tokens) {
            if(r.RuleTokenType == RuleTokenType::NON_TERMINAL) {
                std:: unordered_set<std::string> &new_first = calcFirst(r.getValue());
                
                for(std:: string &s : new_first) {
                    if(s != LAMBDA && cur_first.count(s))
                        cerr<<"Multiple rules on the same input\n", exit(0);
                    
                    cur_first.insert(s);
                    table[state][s] = pr;
                }
                if(!new_first.count(LAMBDA)) break;
            }
            else {
                if(r.RuleTokenType == RuleTokenType::TERMINAL && cur_folow.count(r.getValue()))
                    cerr<<"Multiple rules on the same input\n", exit(0);
                
                cur_first.insert(r.getValue());
                table[state][r.getValue()] = pr;
                if(r.RuleTokenType == RuleTokenType::TERMINAL) break;
            }
        }
    }
    
    return cur_first;
}

std::unordered_set<std::string> PredictiveTable:: calcFollow(std:: string &state) {
    if(!ll1_grammar[state].count(state))
        cerr<< "Undefined token\n", exit(0);
    
    
    if(follow.count(state)) return follow[state];
    
    std::unordered_set<std::string> &cur_follow = follow[state];
    
    for(ProductionRule &pr: ll1_grammar[state]) {
        vector<RuleToken> &tokens = pr.getTokens();
        reverse(tokens.begin(), tokens.end());
        for(RuleToken &r : tokens) {
            if(r.RuleTokenType == RuleTokenType::NON_TERMINAL) {
                std::unordered_set<std::string> &new_follow = calcFollow(r.getValue());
                
                for(std:: string &s : new_follow) {
                    if(cur_folow.count(s))
                        cerr<<"Multiple rules on the same input\n", exit(0);
                    
                    cur_follow.insert(s);
                }
                if(!new_follow.count(LAMBDA)) break;
            }
            else {
                if(r.RuleTokenType == RuleTokenType::TERMINAL && cur_folow.count(r.getValue()))
                    cerr<<"Multiple rules on the same input\n", exit(0);
                
                cur_follow.insert(r.getValue());
                if(r.RuleTokenType == RuleTokenType::TERMINAL) break;
                
            }
        }
        reverse(tokens.begin(), tokens.end());
    }
    
    return cur_folow;
}

std::vector<RuleToken> PredictiveTable:: getTransition(std::string &state, std::string &input) {
    if(!table.count(state) || !table[state].count(input))
        return NULL;
    
    return table[state][input]->getTokens();
}