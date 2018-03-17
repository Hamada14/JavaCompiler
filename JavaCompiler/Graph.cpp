//
//  Graph.cpp
//  JavaCompiler
//
//  Created by Ahmed on 3/14/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#include "Graph.hpp"
#include <stdio.h>

int Graph:: numberOfNodes = 0;

int Graph:: add_node(bool acceptance, string type) {
    int id = ++numberOfNodes;
    adjList[id] = {id, acceptance, type};
    return id;
}


void Graph:: add_edge(int from, int to, string input) {
    if(!adjList.count(from) || !adjList.count(to))
        printf("in add edge function there node isn't in graph added edge from or to");
    adjList[from].transitions.push_back({to,input});
}



unordered_map<int, node>* Graph:: get_nodes() {
    return &adjList;
}
