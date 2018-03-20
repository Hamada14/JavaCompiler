#include "lexical_analyzer/DFA_Minimizer.hpp"

DFA* DFA_Minimizer::get_minimized_DFA() {
	DFA *ret = new DFA;
    State *acc = new State;
    State *non_acc = new State;
    acc->set_acceptance(true), non_acc->set_acceptance(false);
    unordered_map<int,bool> visited;
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
        vector<State*> *cur_partitions = new vector<State*>;
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
	int component_of[number_of_nodes];
	for (State *x : (*partitions)) {
		int cur_node = ret->get_nodes()->add_node(x->get_acceptance(), x->get_type(), x->get_priority());
		for (auto v : (*x->get_nodes())) {
			component_of[v] = cur_node;
		}
	}
	for (State *x : (*partitions)) {
		for (auto v : (*x->get_nodes())) {
			unordered_map<int, node> *cur = g->get_nodes();
			node tmp = (*cur)[v];
			set<string> node_states;
			for (transition nxt : tmp.transitions)
				ret->get_nodes()->add_edge(component_of[v], component_of[nxt.next], nxt.input);
		}
	}
    return ret;
}

void DFA_Minimizer::get_initial_partitions(int v, unordered_map<int,bool> &vis, State *acc, State *non_acc) {
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
    vector<State*> *ret = new vector<State*>;
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
    vector<State*> *ret = new vector<State*>;
    map<set<string> , int> mp;
    int cnt = 1;
    for (auto v : (*state->get_nodes())) {
        unordered_map<int, node> *cur = g->get_nodes();
        node tmp = (*cur)[v];
        set<string> node_states;
        for (transition nxt : tmp.transitions)
            node_states.insert(nxt.input);
        if (!mp[node_states]) {
            mp[node_states] = cnt++;
            State *new_state;
            (*ret).push_back(new_state);
        }
        (*ret)[mp[node_states] - 1]->get_nodes()->insert(v);
    }
    return ret;
}

void DFA_Minimizer::set_state(State *state) {
    bool is_acceptance_state = false;
    int prio = INT_MIN;
    string type;
    set<int> *cur = state->get_nodes();
    for (auto v : *cur) {
        unordered_map<int, node> *cur = g->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
        if (prio < tmp.priority)
            prio = tmp.priority, type = tmp.type;
    }
    state->set_acceptance(is_acceptance_state);
    state->set_type(type);
    state->set_priority(prio);
}
