#include "RegularExpressionTable.hpp"


bool RegularExpressionTable::hasExpression(std::string exp_name) {
        return nfa_map.find(exp_name) != nfa_map.end();
}

NFA* RegularExpressionTable::getExpressionNFA(std::string exp_name) {
        return nfa_map[exp_name];
}
