#include "parser/PredictiveTable.hpp"

#include "Config.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>


PredictiveTable:: PredictiveTable( std::map<std::string, std::vector<ProductionRule> > ll1_grammar, std::string start_state)
: ll1_grammar (ll1_grammar), start_state(start_state) {
    for(auto& it: ll1_grammar) {
        getFirst(it.first);
        getFollow(it.first);
    }
    setPossibleTransitions();
    print_predictive_table = new PrintPredictiveTable(Config::getInstance()->get(Config::PREDICTIVE_TABLE_PATH_KEY));
    std::vector<string> head(1, "Non Terminal");
    for (auto& itr : possible_transitions)
        head.push_back(itr);
    print_predictive_table->printHeader(head);
    printTable();
}

std::string PredictiveTable::getStartState() {
    return start_state;
}

bool PredictiveTable:: checkTerminals(RuleToken &r, std::unordered_set<std::string> &cur, std::string &state, ProductionRule &pr, TYPE type) {
    cur.insert(r.getValue());
    if(type == TYPE::FIRST)
        table[state][r.getValue()] = pr;
    if(r.getType() == RuleTokenType::TERMINAL)
        return false;

    return true;
}

void PredictiveTable::addFollowToTable(std::string state) {
    for(ProductionRule &pr: ll1_grammar[state]) {
        std::vector<RuleToken> tokens = pr.getTokens();
        bool has_lambda = true;
        for(RuleToken &r : tokens) {
            if(r.getType() == RuleTokenType::NON_TERMINAL)
                has_lambda &= getFirst(r.getValue()).count(Constants::LAMBDA);
            else has_lambda &= r.getType() == RuleTokenType::LAMBDA_TERMINAL;
            if(!has_lambda) break;
        }
        if(has_lambda) {
            std::unordered_set<std::string> cur_follow = getFollow(state);
            for(std::string s: cur_follow) {
                if(table[state].count(s) && table[state][s].getTokens() != pr.getTokens() && s != Constants::LAMBDA)
                    std::cerr << "Multiple rules on the same input\n", exit(0);
                table[state][s] = pr;
            }
        }
    }
}

std::unordered_set<std::string> PredictiveTable:: getFirst(std::string state) {
    if(!ll1_grammar.count(state))
        std::cerr<< "Undefined token in getFirst {" << state << "}", exit(-1);

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
                    if(s == Constants::LAMBDA) continue;
                    cur_first.insert(s);
                    table[state][s] = pr;
                }
                has_lambda &= new_first.count(Constants::LAMBDA);
            }
            else has_lambda &= checkTerminals(r, cur_first, state, pr, TYPE::FIRST);
            if(!has_lambda) break;
        }
        if(has_lambda) cur_first.insert(Constants::LAMBDA);
    }

    addFollowToTable(state);
    return cur_first;
}

std::unordered_set<std::string> PredictiveTable:: getFollow(std:: string state) {
    if(!ll1_grammar.count(state))
        std::cerr << "Undefined token in getFollow {" << state << "}", exit(-1);

    if(follow.count(state)) return follow[state];

    std::unordered_set<std::string> &cur_follow = follow[state];
    if(state == start_state)
        cur_follow.insert(Constants::END_OF_INPUT);

    for(auto prs: ll1_grammar) {
        for(ProductionRule pr: prs.second) {
            std::vector<RuleToken> tokens = pr.getTokens();
            for(int i = 0; i < (int)tokens.size() - 1 ; ++i) {
                if(tokens[i].getValue() == state) {
                    std::unordered_set<std::string> cur_first;
                    if(tokens[i+1].getType() == RuleTokenType::NON_TERMINAL) cur_first = getFirst(tokens[i+1].getValue());
                    else cur_first.insert(tokens[i+1].getValue());
                    std::unordered_set<std::string>:: iterator it = cur_first.find(Constants:: LAMBDA);
                    if(it != cur_first.end()) cur_first.erase(it);
                    cur_follow.insert(cur_first.begin(), cur_first.end());
                }
            }
            for(int i = (int) tokens.size()-1 ; i >= 0 ; --i) {
                if(tokens[i].getValue() == state) {
                    std::unordered_set<std::string> new_follow = getFollow(prs.first);
                    cur_follow.insert(new_follow.begin(), new_follow.end());
                }
                if(tokens[i].getType() == RuleTokenType::NON_TERMINAL && !getFirst(tokens[i].getValue()).count(Constants::LAMBDA))
                    break;
                if(tokens[i].getType() == RuleTokenType::TERMINAL)
                    break;
            }
        }
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

void PredictiveTable::setPossibleTransitions() {
    for (auto& itr : ll1_grammar) {
        for (auto& pr : itr.second) {
            for (auto& tk : pr.getTokens()) {
                if (tk.getType() == RuleTokenType::TERMINAL)
                    possible_transitions.insert(tk.getValue());
            }
        }
    }
}

void PredictiveTable::printTable() {
    for (auto& itr : ll1_grammar) {
        std::vector<string> row(1, itr.first);
        for (auto& input : possible_transitions) {
            if (getTransitionType(itr.first, input) == TransitionType::ERROR)
                row.push_back("ERROR");
            if (getTransitionType(itr.first, input) == TransitionType::SYNC)
                row.push_back("SYNC");
            if (getTransitionType(itr.first, input) == TransitionType::LEGAL) {
                std::string nxt = "";
                for (auto& trans : getTransition(itr.first, input))
                    nxt += trans.getValue() + " ";
                row.push_back(nxt);
            }
        }
        print_predictive_table->printData(row);
    }
}
