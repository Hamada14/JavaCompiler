#include "lexical_analyzer/DFA_Builder.hpp"

DFA* DFA_Builder::get_DFA() {
    DFA *ret;
    bool visited[number_of_nodes];
    std::memset(visited, 0, sizeof visited);
    solve_epsillon(start_node, visited);
    unordered_set<int> cur = (*epsillon)[start_node];
    State *first_state;
    for (auto x : cur)
        first_state->get_nodes()->insert(x);
    set_state(first_state);
    first_state->set_id(ret->get_start_node());

    ret->set_start_node(ret->get_nodes()->add_node(first_state->get_acceptance(), first_state->get_type()));
    ret->set_end_node(ret->get_nodes()->add_node(false, ""));

    push_state(first_state);
    subset_construction(*ret);
    return NULL;
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

void DFA_Builder::subset_construction(DFA &ret) {
    string trans;
    while (!stk.empty()) {
        State *cur_state = stk.top();
        stk.pop();
        for (char c = 0; c < char(128); ++c) {
            set<int> next;
            trans = string(&c);
            for (int v : (*(*cur_state).get_nodes())) {
                unordered_map<int, node> *cur = nfa_graph->get_nodes();
                node tmp = (*cur)[v];
                for (transition transitions : tmp.transitions)
                    if (transitions.input == trans)
                        next.insert(transitions.next);
            }
            if (next.size() == 0)
                ret.get_nodes()->add_edge(cur_state->get_id(), ret.get_end_node(), trans);
            else {
                if ((*is_a_state).count(next)) {
                    ret.get_nodes()->add_edge(cur_state->get_id(), (*is_a_state)[next], trans);
                }
                else {
                    State *nxt;
                    nxt->set_nodes(&next), nxt->set_id((int)ret.get_nodes()->get_nodes()->size() + 1);
                    set_state(nxt);
                    ret.get_nodes()->add_node(nxt->get_acceptance(), nxt->get_type());
                    ret.get_nodes()->add_edge(cur_state->get_id(), nxt->get_id(), trans);
                    push_state(nxt);
                }
            }
        }
    }
}


void DFA_Builder::push_state(State *state) {
    if (taken.count(state->get_id()))
        return;
    taken.insert(state->get_id());
    stk.push(state);
}

void DFA_Builder::set_state(State *state) {
    bool is_acceptance_state = false;
    int prio = INT_MAX;
    string type;
    set<int> *cur = state->get_nodes();
    for (auto v : *cur) {
        unordered_map<int, node> *cur = nfa_graph->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
//        if (prio > tmp.prio)
//            prio = tmp.prio, type = tmp.type;
    }
    state->set_acceptance(is_acceptance_state);
    state->set_type(type);
    state->set_priority(prio);
}
