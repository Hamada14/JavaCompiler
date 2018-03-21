#ifndef DFA_BUILDER_HPP_INCLUDED
#define DFA_BUILDER_HPP_INCLUDED

#include "DFA.hpp"
#include "Graph.hpp"
#include "NFA.hpp"
#include "State.hpp"
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <stdio.h>
#include <unordered_map>
#include <unordered_set>

class DFA_Builder {
    private:
        NFA *nfa;
        Graph *nfa_graph;
        int number_of_nodes;
        int start_node;
        int end_node;
        stack<State*> stk;
        unordered_set<int> taken;
        unordered_map<int, unordered_set<int> > *epsillon;
        map<set<int>, int> *is_a_state;
        void solve_epsillon(int v, unordered_map<int,bool> &vis);
        void get_epsillon_closure(int v, unordered_set<int> *result);
        void push_state(State *state);
        void set_state(State *state);
        void connect_edge(State *cur_state, State *next, DFA &ret, string transition);
        void subset_construction(DFA &ret);
    public:
        DFA_Builder(NFA *nfa) {
            this->nfa = nfa;
            this->nfa_graph = nfa->get_graph();
            this->start_node = nfa->get_start_node();
            this->end_node = nfa->get_end_node();
            this->number_of_nodes = (int)(this->nfa_graph->get_nodes()->size());

            epsillon = new unordered_map<int, unordered_set<int> >;
            is_a_state = new map<set<int>, int>;
        }

        DFA *get_DFA();
};

#endif // DFA_BUILDER_HPP_INCLUDED
