//
//  Graph.cpp
//  JavaCompiler
//
//  Created by Ahmed on 3/14/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//

#include "Graph.hpp"

int Graph:: add_node(bool acceptance) {
    id_to_pos[++number_of_nodes] = (int)adjList.size();
    adjList.push_back({number_of_nodes, (int)adjList.size(), acceptance});
    return number_of_nodes;
}


void Graph:: add_edge(int from, int to, string input) {
    if(!id_to_pos.count(from) || !id_to_pos.count(to))
        printf("in add edge function there node isn't in graph added edge from or to");
    adjList[id_to_pos[from]].transitions.push_back({to,input});
}