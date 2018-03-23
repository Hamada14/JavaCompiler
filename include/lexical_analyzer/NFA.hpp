#ifndef NFA_hpp
#define NFA_hpp

#include "Graph.hpp"
#include <stdio.h>

class NFA {
    int start_node, end_node; //node_ids
    Graph g;

    public:
        NFA(char ch);
        NFA(std::string);
        NFA(Graph &g, int start_node, int end_node);
        NFA();
        virtual ~NFA();
        Graph* get_graph();
        int get_start_node();
        int get_end_node();
        int get_priority();
        void set_graph(const Graph &g);
        void set_start_node(int node);
        void set_end_node(int node);
        void set_acceptance(std::string type, int priority);
        virtual NFA* clone();
        NFA* orOperation(NFA &nfa);
        NFA* concatenateOperation (NFA &nfa);
        NFA* asteriskOperation ();
        NFA* plusOperation ();
        static NFA* combine(vector<NFA*> &nfas);

    private:
        void set_priority(int priority);
        void set_type(std::string);
};


#endif /* NFA_hpp */
