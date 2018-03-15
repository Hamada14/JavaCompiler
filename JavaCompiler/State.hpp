#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <stdio.h>
#include <unordered_set>
#include "Graph.hpp"

class State {
    private:
        unordered_set<int> *nodes;
        int id;
    public:
        unordered_set<int> *get_nodes();
        int get_id();
        void set_nodes(unordered_set<int> *n);
        void set_id(int idx);

};

#endif // STATE_HPP_INCLUDED
