#ifndef DFA_H
#define DFA_H

#include "Graph.hpp"

class DFA
{
    public:
        DFA();
        virtual ~DFA();

        int get_start_node() { return start_node; }
        void set_start_node(int val) { start_node = val; }
        int get_end_node() { return end_node; }
        void set_end_node(int val) { end_node = val; }
        Graph *get_nodes() { return g; }
        void set_nodes(Graph *val) { g = val; }

    protected:

    private:
        int start_node;
        int end_node;
        Graph *g;
};

#endif // DFA_H
