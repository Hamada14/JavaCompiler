#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "Graph.hpp"
#include <set>
#include <stdio.h>

class State {
    private:
        set<int> *nodes;
        int id;
        bool is_accepteance;
        string type;
        int priority;
    public:
        set<int> *get_nodes();
        int get_id();
        int get_priority();
        bool get_acceptance();
        string get_type();
        void set_nodes(set<int> *n);
        void set_id(int idx);
        void set_priority(int prio);
        void set_acceptance(bool s);
        void set_type(string s);

};

#endif // STATE_HPP_INCLUDED
