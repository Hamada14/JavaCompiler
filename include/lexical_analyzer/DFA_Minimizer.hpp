#ifndef DFA_MINIMIZER_H_INCLUDED
#define DFA_MINIMIZER_H_INCLUDED

#include "DFA.hpp"
#include "Graph.hpp"
#include "State.hpp"
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>

/*
    the class which minimize DFA
    input of the class is DFA with the graph, start node and end node
    output is a minimal DFA accepting the same expressions as input DFA
*/

class DFA_Minimizer
{
private:
    /*
        DFA Graph which is processed in the class
    */
    Graph* dfa_graph;
    /*
        start node of the graph
    */
    int start_node;
    /*
        end node of the graph
    */
    int end_node;
    /*
        vector of partitions of nodes to construct minimal DFA
    */
    vector<State*>* partitions;
    /*
        Splits all nodes to two partitions : accepting and non accepting partitions
    */
    void get_initial_partitions(int cur, unordered_map<int, bool>& vis, State* acc, State* non_acc);
    /*
        the id of the partition which holds a certain node
    */
    unordered_map<int, int> component_of;
    /*
        Function to separate accepting states by their types to make each type an accepting state
    */
    vector<State*>* separate_accepting_states(State* state);
    /*
        main loop to partitions initial subset of partitions till it become unchanged
    */
    void construct_partitions();
    /*
        Split some partition of nodes to smaller partitions of same nodes
    */
    vector<State*>* construct_new_partitions(State* state);
    /*
        build minimal DFA graph of last set of partitions
    */
    void build_graph(DFA& dfa);
    /*
        Function to give each node equivalent id
    */
    void set_partiotions();
    /*
        helper function to set attributes of a certain state
    */
    void set_state(State* state);

public:
    /*
        Constructor of the class
    */
    DFA_Minimizer(DFA* dfa)
    {
        this->dfa_graph = dfa->get_nodes();
        this->start_node = dfa->get_start_node();
        this->end_node = dfa->get_end_node();
        this->partitions = new vector<State*>;
    }

    /*
        factory of the class which runs the steps of the algorithm,
        build the graph and return minimal DFA
    */
    DFA* get_minimal_DFA();
};

#endif // DFA_MINIMIZER_H_INCLUDED
