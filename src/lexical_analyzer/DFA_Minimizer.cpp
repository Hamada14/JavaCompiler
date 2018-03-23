#include "lexical_analyzer/DFA_Minimizer.hpp"

DFA* DFA_Minimizer::get_minimal_DFA()
{
    DFA* ret = new DFA;

    unordered_map<int, bool> visited;
    State *accepting_partition = new State, *non_accepting_partition = new State;
    get_initial_partitions(start_node, visited, accepting_partition, non_accepting_partition);

    set_state(non_accepting_partition);
    (*partitions).push_back(non_accepting_partition);
    vector<State*>* partitioned = separate_accepting_states(accepting_partition);
    for (State* cur_state : (*partitioned))
        (*partitions).push_back(cur_state);

    construct_partitions();
    build_graph(*ret);

    return ret;
}

void DFA_Minimizer::get_initial_partitions(
    int v, unordered_map<int, bool>& vis, State* acc, State* non_acc)
{
    vis[v] = true;
    unordered_map<int, node>* graph_node = dfa_graph->get_nodes();
    node adjlist = (*graph_node)[v];
    if (adjlist.isAcceptance())
        acc->insert_node(v);
    else
        non_acc->insert_node(v);
    for (transition& x : adjlist.transitions)
        if (!vis[x.next])
            get_initial_partitions(x.next, vis, acc, non_acc);
}

vector<State*>* DFA_Minimizer::separate_accepting_states(State* state)
{
    vector<State*>* diffrent_types = new vector<State*>;
    map<string, int> index;
    int cnt = 1;
    for (int cur_node : (*state->get_nodes()))
    {
        unordered_map<int, node>* graph_nodes = dfa_graph->get_nodes();
        node adjlist = (*graph_nodes)[cur_node];
        if (!index[adjlist.type])
        {
            index[adjlist.type] = cnt++;
            State* new_state = new State;
            (*diffrent_types).push_back(new_state);
        }
        (*diffrent_types)[index[adjlist.type] - 1]->insert_node(cur_node);
    }
    for (State* cur_state : *diffrent_types)
        set_state(cur_state);
    return diffrent_types;
}

void DFA_Minimizer::construct_partitions()
{
    bool changed = true;
    set_partiotions();
    while (changed)
    {
        vector<State*>* cur_partitions = new vector<State*>;
        changed = false;
        for (State* x : (*partitions))
        {
            vector<State*>* parts = construct_new_partitions(x);
            if (parts->size() != 1)
                changed = true;
            for (State* y : (*parts))
                (*cur_partitions).push_back(y);
        }
        partitions = cur_partitions;
        set_partiotions();
    }
}

vector<State*>* DFA_Minimizer::construct_new_partitions(State* state)
{
    vector<State*>* new_partitions = new vector<State*>;
    map<vector<int>, int> index;
    int cnt = 1;
    for (int cur_node : (*state->get_nodes()))
    {
        unordered_map<int, node>* graph_nodes = dfa_graph->get_nodes();
        node adjlist = (*graph_nodes)[cur_node];
        vector<int> next_state(128, -1);
        for (transition& x : adjlist.transitions)
            if (x.input != "/L")
                next_state[x.input[0]] = component_of[x.next];
        if (!index[next_state])
        {
            index[next_state] = cnt++;
            State* new_state = new State;
            (*new_partitions).push_back(new_state);
        }
        (*new_partitions)[index[next_state] - 1]->insert_node(cur_node);
    }
    for (State* cur_state : *new_partitions)
        set_state(cur_state);
    return new_partitions;
}

void DFA_Minimizer::build_graph(DFA& dfa)
{
    for (State* cur_state : (*partitions))
    {
        int cur_node = dfa.get_nodes()->add_node(
            cur_state->get_acceptance(), cur_state->get_type(), cur_state->get_priority());
        cur_state->set_id(cur_node);
        for (int v : (*cur_state->get_nodes()))
            component_of[v] = cur_node;
    }
    dfa.set_start_node(component_of[start_node]);
    dfa.set_end_node(component_of[end_node]);
    Print* minimal_transition_table = new Print(dfa.get_start_node(), "minimal_transition_table.txt");
    minimal_transition_table->printHeader();
    map<pair<pair<int, int>, string>, bool> is_edge;
    for (State* cur_state : (*partitions))
    {
        vector<int> data(128, component_of[end_node]);
        for (auto v : (*cur_state->get_nodes()))
        {
            unordered_map<int, node>* graph_nodes = dfa_graph->get_nodes();
            node adjlist = (*graph_nodes)[v];
            for (transition nxt : adjlist.transitions)
            {
                auto edge
                    = make_pair(make_pair(component_of[v], component_of[nxt.next]), nxt.input);
                if (!is_edge[edge])
                {
                    data[nxt.input[0]] = component_of[nxt.next];
                    dfa.get_nodes()->add_edge(component_of[v], component_of[nxt.next], nxt.input);
                }
                is_edge[edge] = true;
            }
        }
        minimal_transition_table->pirnt_data(cur_state->get_id(), cur_state->get_type(),
            cur_state->get_priority(), cur_state->get_acceptance(), data);
    }
    minimal_transition_table->close_file();
}

void DFA_Minimizer::set_partiotions()
{
    int partition_id = 1;
    for (State* cur_state : *partitions)
    {
        cur_state->set_id(partition_id++);
        for (int cur_node : (*cur_state->get_nodes()))
            component_of[cur_node] = cur_state->get_id();
    }
}

void DFA_Minimizer::set_state(State* state)
{
    bool is_acceptance_state = false;
    int prio = INT_MIN;
    string type;
    set<int>* cur = state->get_nodes();
    for (auto v : *cur)
    {
        unordered_map<int, node>* cur = dfa_graph->get_nodes();
        node tmp = (*cur)[v];
        is_acceptance_state |= tmp.acceptance;
        if (prio < tmp.priority)
            prio = tmp.priority, type = tmp.type;
    }
    state->set_acceptance(is_acceptance_state);
    state->set_type(type);
    state->set_priority(prio);
}
