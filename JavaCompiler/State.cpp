#include "State.hpp"

set<int>* State::get_nodes() {
    return nodes;
}

int State::get_id() {
    return id;
}

void State::set_nodes(set<int> *n) {
    nodes = n;
}

void State::set_id(int idx) {
    id = idx;
}
