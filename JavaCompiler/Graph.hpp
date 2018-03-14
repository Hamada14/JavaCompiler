//
//  Graph.hpp
//  JavaCompiler
//
//  Created by Ahmed on 3/14/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#ifndef Graph_hpp
#define Graph_hpp
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;


struct transition {
    int next; // node id;
    string input; // string because /L == lamda
};


// it can be pool design pattern for better performance
struct node {
    int id; // we still need valid mapping between id, pos
    bool acceptance;
    string type;
    vector< transition > transitions;
};

class Graph {
    
    static int numberOfNodes;
    unordered_map<int, node> adjList;
    
    public:
        int add_node(bool acceptance, string type);  // returns node id
        void add_edge(int from, int to, string input); //function parameters(from, to) are node ids
        unordered_map<int, node>* get_nodes();
    
};



#endif /* Graph_hpp */
