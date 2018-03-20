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

class DFA_Minimizer {
    private:
        int start_node;
        int end_node;
        int number_of_nodes;
        Graph *g;
        vector<State*> *partitions;
        void get_initial_partitions(int cur, unordered_map<int,bool> &vis, State *acc, State *non_acc);
        vector<State*>* construct_new_partition_by_type(State *state);
        vector<State*>* construct_new_partition_by_transtion(State *state);
        void set_state(State *state);

    public:
        DFA_Minimizer(DFA *dfa) {
            this->g = dfa->get_nodes();
            this->start_node = dfa->get_start_node();
            this->end_node = dfa->get_end_node();
            this->number_of_nodes = (int) (this->g->get_nodes()->size());
        }

        DFA *get_minimized_DFA();
};

#endif // DFA_MINIMIZER_H_INCLUDED
