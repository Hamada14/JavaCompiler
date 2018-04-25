//
//  PredictiveTable.cpp
//
//  Created by Ahmed on 4/25/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#include "PredictiveTable.hpp"


#include <algorithm>
#include <cassert>
#include <iostream>


PredictiveTable( std::map<std::string, std::vector<ProductionRule> > & ll1_grammar): ll1_grammar (ll1_grammar) {
    for(auto &it: ll1_grammar)
        getFirst(it.f), getFollow(it.f);
    
}

bool PredictiveTable:: checkTerminals(RuleToken &r, std::unordered_set<std::string> &cur, std::string &state, TYPE type) {
    if(type == TYPE::FIRST && r.RuleTokenType != Constants::RuleTokenType::TERMINAL && cur.count(r.getValue()))
        std::cerr<<"Multiple rules on the same input\n", exit(0);
    
    cur.insert(r.getValue());
    
    if(type == TYPE::FIRST) table[state][r.getValue()] = pr;
    if(type == TYPE::FIRST && r.RuleTokenType == Constants::RuleTokenType::TERMINAL)
        return false;
    
    return true;
}



std::unordered_set<std::string> PredictiveTable:: getFirst(std::string &state) {
    if(!ll1_grammar.count(state))
        std::cerr<< "Undefined token\n", exit(0);
    
    if(first.count(state)) return first[state];
    
    std:: unordered_set<std::string> &cur_first = first[state];
    for(ProductionRule &pr: ll1_grammar[state]) {
        vector<RuleToken> &tokens = pr.getTokens();
        bool has_lambda = true;
        for(RuleToken &r : tokens) {
            if(r.RuleTokenType == RuleTokenType::NON_TERMINAL) {
                std:: unordered_set<std::string> &new_first = calcFirst(r.getValue());
                
                for(std:: string &s : new_first) {
                    if(s == Constants::LAMBDA) continue;
                    if(cur_first.count(s))
                        cerr<<"Multiple rules on the same input\n", exit(0);
                    
                    cur_first.insert(s);
                    table[state][s] = pr;
                }
                
                has_lambda &= new_first.count(Constants::LAMBDA);
                if(!new_first.count(Constants::LAMBDA)) break;
            }
            else {
                if(!check_terminal(r, cur_first, state, TYPE::FIRST)) {
                    has_lambda = false;
                    break;
                }
            }
        }
        if(has_lambda){
            cur_first.insert(Constants::LAMBDA);
            std::unordered_set<std::string> cur_follow = getFollow(state);
            for(string &s: cur_follow) {
                if(table[state].count(s) && table[state][s] != pr.getTokens())
                    cerr<<"Multiple rules on the same input\n", exit(0);
                table[state][s] = pr;
            }
        }
    }
    return cur_first;
}

void PredictiveTable:: calcRHSFollow(std::unordered_set<std::string> &cur_follow, std::string &state) {
    for(auto &pr: ll1_grammar) {
        vector<RuleToken> &tokens = pr.second.getTokens();
        for(int i = 0; i < (int)tokens.size()-1 ; ++i) {
            if(tokens[i] == state) {
                std::unordered_set<std::string> cur_first = getFirst(tokens[i+1]);
                std::unordered_set<std::string>:: iterator it = cur_first.find(Constants:: LAMBDA);
                if(it != cur_first.end()) cur_first.erase(it);
                cur_follow.insert(cur_first.begin(), cur_first.end());
            }
        }
    }
}

// how to know start variable to add $ in its follow ???
std::unordered_set<std::string> PredictiveTable:: getFollow(std:: string &state) {
    if(!ll1_grammar.count(state))
        cerr<< "Undefined token\n", exit(0);
    
    if(follow.count(state)) return follow[state];
    
    std::unordered_set<std::string> &cur_follow = follow[state];
    calcRHSFollow(cur_follow, state);
    
    for(ProductionRule &pr: ll1_grammar[state]) {
        vector<RuleToken> &tokens = pr.getTokens();
        reverse(tokens.begin(), tokens.end());
        for(RuleToken &r : tokens) {
            if(r.RuleTokenType == RuleTokenType::NON_TERMINAL) {
                std::unordered_set<std::string> &new_follow = getFollow(r.getValue());
                cur_follow.insert(new_follow.begin(), new_follow.end());
                if(!getFirst(r.getValue()).count(Constants::LAMBDA)) break;
            }
            else check_terminal(r, cur_follow, state, TYPE::FOLLOW);
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