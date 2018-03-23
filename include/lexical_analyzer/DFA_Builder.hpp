#ifndef DFA_BUILDER_HPP_INCLUDED
#define DFA_BUILDER_HPP_INCLUDED

#include "DFA.hpp"
#include "Graph.hpp"
#include "NFA.hpp"
#include "State.hpp"
#include "Print.hpp"
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>

/*
    the main class in building DFA
    input of the class is NFA with the graph, start node and end node
    output is a DFA accepting the same expressions as input NFA
*/

class DFA_Builder
{
private:
    /*
        NFA Graph which is processed in the class
    */
    Graph* nfa_graph;
    /*
        start node of the graph
    */
    int start_node;
    /*
        end node of the graph
    */
    int end_node;
    /*
        determine if epsillon closure for a node is computed
    */
    unordered_map<int, bool> epsillon_computed;
    /*
        epsillon closure of each node of the NFA graph
    */
    unordered_map<int, unordered_set<int> >* epsillon;
    /*
        stack which hold states that should be built in the DFA
    */
    stack<State*> stk;
    /*
        taken array to mark visited states
    */
    unordered_map<int, bool> taken;
    /*
        array to check if a state is formed before or not
    */
    map<set<int>, int>* is_a_state;
    /*
        function to compute the epsillon closure of a certain node
    */
    void solve_epsillon(int v);
    /*
        A DFS function which get epsillon closure for a certain node
    */
    void get_epsillon_closure(int v, unordered_set<int>* result);
    /*
        Main algorithm of forming DFA
        start with the state of the start node of the graph
        pushing all possible states to the graph
    */
    void subset_construction(DFA& ret);
    /*
        a functions to get all possible transitions in a graph
    */
    vector<string> get_possible_transitions();
    /*
        search all possible transitions for a certain transitions and the add the epsillon closure
            of it to the graph
    */
    void search_transtion(int node_id, State* next, string trans);
    /*
        connect two states if the is a valid edge between 'em in the graph
        or add edge to PHI state otherwise
    */
    void connect_edge(
        State* cur_state, State* next, DFA& ret, string transition, vector<int>& data);
    /*
        helper function which pushes a state to the stack if this state is not pushed before
    */
    void push_state(State* state);
    /*
        helper function to set attributes of a certain state
    */
    void set_state(State* state);

public:
    /*
        Constructor of the class
    */
    DFA_Builder(NFA* nfa)
    {
        this->nfa_graph = nfa->get_graph();
        this->start_node = nfa->get_start_node();
        this->end_node = nfa->get_end_node();
        epsillon = new unordered_map<int, unordered_set<int> >;
        is_a_state = new map<set<int>, int>;
    }

    /*
        factory of the class which runs the steps of the algorithm,
        build the graph and return DFA
    */
    DFA* get_DFA();
};

#endif // DFA_BUILDER_HPP_INCLUDED
