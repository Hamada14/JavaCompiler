//
//  NFA.hpp
//  JavaCompiler
//
//  Created by Ahmed on 3/13/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#ifndef NFA_hpp
#define NFA_hpp

#include <stdio.h>
#include "Graph.hpp"

class NFA {
    int start_node, end_node; //node_ids
    int priority;
    Graph g;
    
    public:
        NFA(char ch, string type);
        NFA(Graph &g, int start_node, int end_node, int priority);
        Graph* get_graph();
        int get_start_node();
        int get_end_node();
        int get_priority();
        void set_graph(const Graph &g);
        void set_start_node(int node);
        void set_end_node(int node);
        void set_priority(int priority);
        virtual NFA clone();
        NFA operator | (NFA &nfa);
        NFA cascade (NFA &nfa);
        NFA operator * ();
        NFA operator + ();

    
    
};


#endif /* NFA_hpp */
