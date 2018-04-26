#include "..\..\include\parser/PredictiveTable.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>


PredictiveTable:: PredictiveTable( std::map<std::string, std::vector<ProductionRule> > & ll1_grammar, std::string start_state)
: ll1_grammar (ll1_grammar), start_state(start_state) {
    for(auto& it: ll1_grammar)
        getFirst(it.first), getFollow(it.first);
}

bool PredictiveTable:: checkTerminals(RuleToken &r, std::unordered_set<std::string> &cur, std::string &state, ProductionRule &pr, TYPE type) {
    if(type == TYPE::FIRST && r.getType() != RuleTokenType::TERMINAL && cur.count(r.getValue()))
        std::cerr<<"Multiple rules on the same input\n", exit(0);

    cur.insert(r.getValue());

    if(type == TYPE::FIRST)
        table[state][r.getValue()] = pr;
    if(type == TYPE::FIRST && r.getType() == RuleTokenType::TERMINAL)
        return false;

    return true;
}



std::unordered_set<std::string> PredictiveTable:: getFirst(std::string state) {
    if(!ll1_grammar.count(state))
        std::cerr<< "Undefined token\n", exit(0);

    if(first.count(state))
        return first[state];

    std::unordered_set<std::string> &cur_first = first[state];
    for(ProductionRule &pr: ll1_grammar[state]) {
        std::vector<RuleToken> tokens = pr.getTokens();
        bool has_lambda = true;
        for(RuleToken &r : tokens) {
            if(r.getType() == RuleTokenType::NON_TERMINAL) {
                std:: unordered_set<std::string> new_first = getFirst(r.getValue());

                for(std::string s : new_first) {
                    if(s == Constants::LAMBDA)
                        continue;
                    if(cur_first.count(s))
                        std::cerr << "Multiple rules on the same input\n", exit(0);

                    cur_first.insert(s);
                    table[state][s] = pr;
                }

                has_lambda &= new_first.count(Constants::LAMBDA);
                if(!new_first.count(Constants::LAMBDA)) break;
            }
            else {
                if(!checkTerminals(r, cur_first, state, pr, TYPE::FIRST)) {
                    has_lambda = false;
                    break;
                }
            }
        }
        if(has_lambda){
            cur_first.insert(Constants::LAMBDA);
            std::unordered_set<std::string> cur_follow = getFollow(state);
            for(std::string s: cur_follow) {
                if(table[state].count(s) && table[state][s].getTokens() != pr.getTokens())
                    std::cerr << "Multiple rules on the same input\n", exit(0);
                table[state][s] = pr;
            }
        }
    }
    return cur_first;
}

void PredictiveTable:: calcRHSFollow(std::unordered_set<std::string> &cur_follow, std::string &state) {
    for(auto prs: ll1_grammar) {
        for(ProductionRule pr: prs.second) {
            std::vector<RuleToken> tokens = pr.getTokens();
            for(int i = 0; i < (int)tokens.size() - 1 ; ++i) {
                if(tokens[i].getValue() == state) {
                    std::unordered_set<std::string> cur_first = getFirst(tokens[i+1].getValue());
                    std::unordered_set<std::string>:: iterator it = cur_first.find(Constants:: LAMBDA);
                    if(it != cur_first.end()) cur_first.erase(it);
                    cur_follow.insert(cur_first.begin(), cur_first.end());
                }
            }
        }
    }
}


std::unordered_set<std::string> PredictiveTable:: getFollow(std:: string state) {
    if(!ll1_grammar.count(state))
        std::cerr << "Undefined token\n", exit(0);

    if(follow.count(state)) return follow[state];

    std::unordered_set<std::string> &cur_follow = follow[state];
    calcRHSFollow(cur_follow, state);
    if(state == start_state)
        cur_follow.insert(Constants::END_OF_INPUT);

    for(ProductionRule &pr: ll1_grammar[state]) {
        std::vector<RuleToken> tokens = pr.getTokens();
        std::reverse(tokens.begin(), tokens.end());
        for(RuleToken &r : tokens) {
            if(r.getType() == RuleTokenType::NON_TERMINAL) {
                std::unordered_set<std::string> new_follow = getFollow(r.getValue());
                cur_follow.insert(new_follow.begin(), new_follow.end());
                if(!getFirst(r.getValue()).count(Constants::LAMBDA)) break;
            }
            else
                checkTerminals(r, cur_follow, state, pr, TYPE::FOLLOW);
        }
        reverse(tokens.begin(), tokens.end());

    }

    return cur_follow;
}


std::vector<RuleToken> PredictiveTable::getTransition(std::string state, std::string input) {
    if(!table.count(state) || !table[state].count(input))
        return std::vector<RuleToken>();
    return table[state][input].getTokens();
}

TransitionType PredictiveTable::getTransitionType(std::string state, std::string input) {
    if(table[state].count(input))
        return TransitionType::LEGAL;

    if(getFollow(state).count(input))
        return TransitionType::SYNC;

    return TransitionType::ERROR;
}
