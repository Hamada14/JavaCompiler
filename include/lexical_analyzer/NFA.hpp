#ifndef NFA_hpp
#define NFA_hpp

#include "Graph.hpp"
#include <stdio.h>

class NFA {
    int start_node, end_node; //node_ids
    Graph g;

    public:
        NFA(char ch);
        NFA(Graph &g, int start_node, int end_node, int priority);
        NFA();
        virtual ~NFA();
        Graph* get_graph();
        int get_start_node();
        int get_end_node();
        int get_priority();
        void set_graph(const Graph &g);
        void set_start_node(int node);
        void set_end_node(int node);
        void set_priority(int priority);
        void set_type(std::string);
        virtual NFA* clone();
        NFA* orOperation(NFA &nfa);
        NFA* concatenateOperation (NFA &nfa);
        NFA* asteriskOperation ();
        NFA* plusOperation ();



};


#endif /* NFA_hpp */
