#include "RegularExpressionTable.hpp"

RegularExpressionTable::~RegularExpressionTable() {
    clear();
}

bool RegularExpressionTable::hasExpression(std::string exp_name) {
        return nfa_map.find(exp_name) != nfa_map.end();
}

NFA* RegularExpressionTable::getExpressionNFA(std::string exp_name) {
        return nfa_map[exp_name];
}

void RegularExpressionTable::addExpressionNFA(std::string exp_name, NFA* nfa) {
        nfa_map[exp_name] = nfa;
}

void RegularExpressionTable::clear() {
    nfa_map.clear();
    std::vector<NFA*> result = getValues();
    for(NFA* nfa : result)
        delete nfa;
}

std::vector<NFA*> RegularExpressionTable::getValues() {
    std::vector<NFA*> result;
    for(map<std::string, NFA*>::iterator it = nfa_map.begin(); it != nfa_map.end(); it++) {
        result.push_back(it->second);
    }
    return result;
}
