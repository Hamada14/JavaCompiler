#include "lexical_analyzer/State.hpp"

set<int>* State::get_nodes() {
    return nodes;
}

int State::get_id() {
    return id;
}

int State::get_priority() {
    return priority;
}

string State::get_type() {
    return type;
}

bool State::get_acceptance() {
    return is_accepteance;
}

void State::set_nodes(set<int> *n) {
    nodes = n;
}

void State::insert_node(int n) {
    nodes->insert(n);
}

void State::set_id(int idx) {
    id = idx;
}

void State::set_priority(int prio) {
    priority = prio;
}

void State::set_type(string s) {
    type = s;
}

void State::set_acceptance(bool s) {
    is_accepteance = s;
}
