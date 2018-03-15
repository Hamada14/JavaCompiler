#include "State.hpp"

unordered_set<int>* State::get_nodes() {
    return nodes;
}

int State::get_id() {
    return id;
}

void State::set_nodes(unordered_set<int> *n) {
    nodes = n;
}

void State::set_id(int idx) {
    id = idx;
}
