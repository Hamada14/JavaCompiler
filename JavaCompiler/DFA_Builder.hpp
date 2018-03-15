#ifndef DFA_BUILDER_H_INCLUDED
#define DFA_BUILDER_H_INCLUDED

#include <stdio.h>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <cstring>
#include <iostream>
#include "Graph.hpp"
#include "NFA.hpp"

class DFA_Builder {
    private:
        NFA *nfa;
        Graph *nfa_graph;
        int start_node;
        int end_node;
        int number_of_nodes;
        stack<int> stk;
        unordered_set<int> taken;
        unordered_map<int, unordered_set<int> > *epsillon;
        void solve_epsillon(int v, bool *vis);
        void get_epsillon_closure(int v, unordered_set<int> *result);
        void push_state(int state);
        void subset_construction(Graph &ret);
    public:
        DFA_Builder(NFA *nfa) {
            this->nfa = nfa;
            this->nfa_graph = nfa->get_graph();
            this->start_node = nfa->get_start_node();
            this->end_node = nfa->get_end_node();
            this->number_of_nodes = this->nfa_graph->get_nodes()->size();
        }

        Graph *get_DFA();
};

#endif // DFA_BUILDER_H_INCLUDED
