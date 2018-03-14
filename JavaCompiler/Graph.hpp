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
    int id, pos; // we still need valid mapping between id, pos
    bool acceptance;
    vector< transition > transitions;
};

class Graph {
    
    static int number_of_nodes;
    vector<node> adjList;
    unordered_map<int, int> id_to_pos;
    
    public:
        int add_node(bool acceptance);  // returns node id
        void add_edge(int from, int to, string input); //function parameters(from, to) are node ids
    
};



#endif /* Graph_hpp */
