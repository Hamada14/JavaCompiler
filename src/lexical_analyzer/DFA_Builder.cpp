#include "lexical_analyzer/DFA_Builder.hpp"

DFA* DFA_Builder::get_DFA()
{
    DFA* ret = new DFA;
    unordered_map<int, bool> visited;

    solve_epsillon(start_node, visited);

    unordered_set<int> fitst_state_nodes = (*epsillon)[start_node];
    State* first_state = new State;
    for (auto cur_node : fitst_state_nodes)
        first_state->insert_node(cur_node);

    set_state(first_state);
    ret->set_start_node(ret->get_nodes()->add_node(
        first_state->get_acceptance(), first_state->get_type(), first_state->get_priority()));
    first_state->set_id(ret->get_start_node());

    ret->set_end_node(ret->get_nodes()->add_node(false, "", -(1 << 20)));

    push_state(first_state);
    subset_construction(*ret);
    return ret;
}

void DFA_Builder::solve_epsillon(int v, unordered_map<int, bool>& vis)
{
    vis[v] = true;
    unordered_set<int>* eps = new unordered_set<int>();
    get_epsillon_closure(v, eps);
    (*epsillon)[v] = (*eps);
    unordered_map<int, node>* graph_nodes = nfa_graph->get_nodes();
    node adjlist = (*graph_nodes)[v];
    for (transition& x : adjlist.transitions)
        if (!vis[x.next] && x.input != "/L")
            solve_epsillon(x.next, vis);
    for (transition& x : adjlist.transitions)
        if (!vis[x.next] && x.input == "/L")
            solve_epsillon(x.next, vis);
}

void DFA_Builder::get_epsillon_closure(int v, unordered_set<int>* result)
{
    result->insert(v);
    unordered_map<int, node>* graph_nodes = nfa_graph->get_nodes();
    node adjlist = (*graph_nodes)[v];
    for (transition& x : adjlist.transitions)
        if (!(*result).count(x.next) && x.input == "/L")
            get_epsillon_closure(x.next, result);
}

void DFA_Builder::subset_construction(DFA& ret)
{
    vector<string> possible_transitions = get_possible_transitions();
    while (!stk.empty())
    {
        State* cur_state = stk.top();
        stk.pop();
        for (auto trans : possible_transitions)
        {
            State* next = new State;
            for (int state_node : (*cur_state->get_nodes()))
                search_transtion(state_node, next, trans);
            connect_edge(cur_state, next, ret, trans);
        }
    }
}

vector<string> DFA_Builder::get_possible_transitions()
{
    string trans;
    vector<string> possible_transitions;
    possible_transitions.push_back("/L");
    for (unsigned char c = 0; c < 128; ++c)
    {
        trans = "", trans += c;
        possible_transitions.push_back(trans);
    }
    return possible_transitions;
}

void DFA_Builder::search_transtion(int node_id, State* next, string trans)
{
    unordered_map<int, node>* graph_nodes = nfa_graph->get_nodes();
    node adjlist = (*graph_nodes)[node_id];
    for (transition& transitions : adjlist.transitions)
    {
        if (transitions.input == trans)
        {
            unordered_set<int> eps = (*epsillon)[transitions.next];
            for (auto v : eps)
                next->insert_node(v);
        }
    }
}

void DFA_Builder::connect_edge(State* cur_state, State* next, DFA& ret, string transition)
{
    if (next->get_nodes()->size() == 0)
        ret.get_nodes()->add_edge(cur_state->get_id(), ret.get_end_node(), transition);
    else
    {
        set<int>* nodes_of_state = next->get_nodes();
        if ((*is_a_state).count(*nodes_of_state))
            ret.get_nodes()->add_edge(
                cur_state->get_id(), (*is_a_state)[*nodes_of_state], transition);
        else
        {
            set_state(next);
            next->set_id(ret.get_nodes()->add_node(
                next->get_acceptance(), next->get_type(), next->get_priority()));
            ret.get_nodes()->add_edge(cur_state->get_id(), next->get_id(), transition);
            push_state(next);
        }
    }
}

void DFA_Builder::push_state(State* state)
{
    if (taken[state->get_id()])
        return;
    taken[state->get_id()] = true;
    (*is_a_state)[*(state->get_nodes())] = state->get_id();
    stk.push(state);
}

void DFA_Builder::set_state(State* state)
{
    bool is_acceptance_state = false;
    int prio = INT_MIN;
    string type;
    set<int>* cur = state->get_nodes();
    for (auto v : *cur)
    {
        unordered_map<int, node>* cur = nfa_graph->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
        if (prio < tmp.priority)
            prio = tmp.priority, type = tmp.type;
    }
    state->set_acceptance(is_acceptance_state);
    state->set_type(type);
    state->set_priority(prio);
}
