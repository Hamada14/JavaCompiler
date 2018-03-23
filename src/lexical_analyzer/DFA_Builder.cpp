#include "lexical_analyzer/DFA_Builder.hpp"

DFA* DFA_Builder::get_DFA()
{
    DFA* ret = new DFA;

    solve_epsillon(start_node);
    unordered_set<int> first_state_nodes = (*epsillon)[start_node];
    State* first_state = new State;
    first_state->get_nodes()->insert(first_state_nodes.begin(), first_state_nodes.end());

    set_state(first_state);
    ret->set_start_node(ret->get_nodes()->add_node(
        first_state->get_acceptance(), first_state->get_type(), first_state->get_priority()));
    first_state->set_id(ret->get_start_node());

    ret->set_end_node(ret->get_nodes()->add_node(false, "", -(infi)));
    push_state(first_state);
    subset_construction(*ret);
    return ret;
}

void DFA_Builder::solve_epsillon(int v)
{
    unordered_set<int>* eps = new unordered_set<int>();
    get_epsillon_closure(v, eps);
    (*epsillon)[v] = (*eps);
    epsillon_computed[v] = true;
}

void DFA_Builder::get_epsillon_closure(int v, unordered_set<int>* result)
{
    result->insert(v);
    vector<node>* graph_nodes = nfa_graph->get_nodes();
    vector<int> &tr = (*graph_nodes)[v].transitions[lambda];
    for(int n: tr)
        if(!(*result).count(n))
            get_epsillon_closure(n, result);

}



void DFA_Builder::subset_construction(DFA& ret)
{
    vector<int> possible_transitions = get_possible_transitions();
    while (!stk.empty())
    {
        State* cur_state = stk.top();
        stk.pop();
        for (int trans : possible_transitions)
        {
            State* next = new State;
            for (int state_node : (*cur_state->get_nodes()))
                search_transtion(state_node, next, trans);
            connect_edge(cur_state, next, ret, trans);
        }
    }
}

vector<int> DFA_Builder::get_possible_transitions()
{
    vector<int> possible_transitions;
    for (int c = 0; c < MAX_INPUT_SIZE; ++c)
        possible_transitions.push_back(c);

    return possible_transitions;
}

void DFA_Builder::search_transtion(int node_id, State* next, int trans)
{
    vector<int>* cur_nodes = nfa_graph->get_nodes_of_transitions(node_id, trans);
    for(int cur_node : *cur_nodes) {
        if (!epsillon_computed[cur_node])
            solve_epsillon(cur_node);
        unordered_set<int> eps = (*epsillon)[cur_node];
        next->get_nodes()->insert(eps.begin(), eps.end());
    }
}

void DFA_Builder::connect_edge(State* cur_state, State* next, DFA& ret, int transition)
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
        vector<node>* cur = nfa_graph->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
        if (prio < tmp.priority)
            prio = tmp.priority, type = tmp.type;
    }
    state->set_acceptance(is_acceptance_state);
    state->set_type(type);
    state->set_priority(prio);
}
