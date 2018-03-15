#include "DFA_Builder.hpp"

Graph* DFA_Builder::get_DFA() {
    bool visited[number_of_nodes];
    std::memset(visited, 0, sizeof visited);
    solve_epsillon(start_node, visited);
    bool is_acceptance_state = false;
    int type = INT_MAX;
    unordered_set<int> cur = (*epsillon)[start_node];
    // missing type
    for (auto v : cur) {
        unordered_map<int, node> *cur = nfa_graph->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
    }
    Graph *ret;
    g_start = ret->add_node(is_acceptance_state, "");
    g_end = ret->add_node(false, "");
    State *cur_state;
    set<int> *first_set;
    for (auto x : cur)
        first_set->insert(x);
    cur_state->set_id(g_start), cur_state->set_nodes(first_set);
    push_state(cur_state);
    subset_construction(*ret);
}

void DFA_Builder::get_epsillon_closure(int v, unordered_set<int> *result) {
    result->insert(v);
    unordered_map<int, node> *cur = nfa_graph->get_nodes();
    node tmp = (*cur)[v];
    for (transition &x: tmp.transitions) {
        if (!(*result).count(x.next) && x.input == "/L")
            get_epsillon_closure(x.next, result);
    }
}

void DFA_Builder::solve_epsillon(int v, bool *vis) {
    vis[v] = true;
    unordered_set<int> *eps = new unordered_set<int>();
    get_epsillon_closure(v, eps);
    (*epsillon)[v] = (*eps);
    unordered_map<int, node> *cur = nfa_graph->get_nodes();
    node tmp = (*cur)[v];
    for (transition &x: tmp.transitions) {
        if (!vis[x.next] && x.input != "/L")
            solve_epsillon(x.next, vis);
    }
}

void DFA_Builder::push_state(State *state) {
    if (taken.count(state->get_id()))
        return;
    taken.insert(state->get_id());
    stk.push(state);
}

void DFA_Builder::subset_construction(Graph &ret) {
    while (!stk.empty()) {
        State *cur_state = stk.top();
        stk.pop();
        for (char c = 0; c < 128; ++c) {
            set<int> next;
            string trans;
            trans += c;
            for (int v : (*(*cur_state).get_nodes())) {
                unordered_map<int, node> *cur = nfa_graph->get_nodes();
                node tmp = (*cur)[v];
                for (transition transitions : tmp.transitions)
                    if (transitions.input == trans)
                        next.insert(transitions.next);
            }
            if (next.size() == 0)
                ret.add_edge(cur_state->get_id(), g_end, trans);
            else {
                if ((*is_a_state).count(next)) {
                    ret.add_edge(cur_state->get_id(), (*is_a_state).count(next), "");
                }
                else {
                    State *nxt;
                    nxt->set_nodes(&next), nxt->set_id(ret.get_nodes()->size() + 1);
                    ret.add_node(nxt->get_id(), "");
                    ret.add_edge(cur_state->get_id(), nxt->get_id(), "");
                    push_state(nxt);
                }
            }
        }
    }
}
