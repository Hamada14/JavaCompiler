#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include <stdio.h>
#include <set>
#include "Graph.hpp"

class State {
    private:
        set<int> *nodes;
        int id;
    public:
        set<int> *get_nodes();
        int get_id();
        void set_nodes(set<int> *n);
        void set_id(int idx);

};

#endif // STATE_HPP_INCLUDED
