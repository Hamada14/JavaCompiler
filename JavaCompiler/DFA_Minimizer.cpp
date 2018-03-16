#include "DFA_Minimizer.h"

DFA* DFA_Minimizer::get_minimized_DFA() {
    State *acc, *non_acc;
    acc->set_acceptance(true), non_acc->set_acceptance(false);
    bool visited[number_of_nodes];
    memset(visited, 0, sizeof visited);
    get_initial_partitions(start_node, visited, acc, non_acc);
    set_state(non_acc);
    (*partitions).push_back(non_acc);
    vector<State*> *partitioned = construct_new_partition_by_type(acc);
    for (State *x : (*partitioned)) {
        set_state(x);
        (*partitions).push_back(x);
    }
    bool changed = true;
    while (changed) {
        vector<State*> *cur_partitions;
        changed = false;
        for (State *x : (*partitions)) {
            vector<State*> *parts = construct_new_partition_by_transtion(x);
            if (parts->size() != 1)
                changed = true;
            for (State *y : (*parts)) {
                set_state(y);
                (*cur_partitions).push_back(y);
            }
        }
        partitions = cur_partitions;
    }
}

void DFA_Minimizer::get_initial_partitions(int v, bool *vis, State *acc, State *non_acc) {
    vis[v] = true;
    unordered_map<int, node> *cur = g->get_nodes();
    node tmp = (*cur)[v];
    if (tmp.isAcceptance())
        acc->get_nodes()->insert(v);
    else
        non_acc->get_nodes()->insert(v);
    for (transition &x: tmp.transitions) {
        if (!vis[x.next])
            get_initial_partitions(x.next, vis, acc, non_acc);
    }
}

vector<State*>* DFA_Minimizer::construct_new_partition_by_type(State *state) {
    vector<State*> *ret;
    map<string, int> mp;
    int cnt = 1;
    for (auto v : (*state->get_nodes())) {
        unordered_map<int, node> *cur = g->get_nodes();
        node tmp = (*cur)[v];
        if (!mp[tmp.type]) {
            mp[tmp.type] = cnt++;
            State *new_state;
            (*ret).push_back(new_state);
        }
        (*ret)[mp[tmp.type] - 1]->get_nodes()->insert(v);
    }
    return ret;
}

vector<State*>* DFA_Minimizer::construct_new_partition_by_transtion(State *state) {
    vector<State*> *ret;
    map<string, int> mp;
    int cnt = 1;
    for (auto v : (*state->get_nodes())) {
        unordered_map<int, node> *cur = g->get_nodes();
        node tmp = (*cur)[v];
        if (!mp[tmp.type]) {
            mp[tmp.type] = cnt++;
            State *new_state;
            (*ret).push_back(new_state);
        }
        (*ret)[mp[tmp.type] - 1]->get_nodes()->insert(v);
    }
    return ret;
}

void DFA_Minimizer::set_state(State *state) {
    bool is_acceptance_state = false;
    int prio = INT_MAX;
    string type;
    set<int> *cur = state->get_nodes();
    for (auto v : *cur) {
        unordered_map<int, node> *cur = g->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
//        if (prio > tmp.prio)
//            prio = tmp.prio, type = tmp.type;
    }
    state->set_acceptance(is_acceptance_state);
    state->set_type(type);
    state->set_priority(prio);
}
