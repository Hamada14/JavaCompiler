#include "lexical_analyzer/Graph.hpp"
#include <stdio.h>

int Graph:: numberOfNodes = 0;

int Graph:: add_node(bool acceptance, string type) {
    int id = ++numberOfNodes;
    adjList[id] = {id, acceptance, type};
    return id;
}

int Graph:: add_node(bool acceptance, string type, int priority) {
    int id = ++numberOfNodes;
    adjList[id] = {id, acceptance, type, priority};
    return id;
}


void Graph:: add_edge(int from, int to, string input) {
    if(!adjList.count(from) || !adjList.count(to))
        printf("in add edge function there node isn't in graph added edge from or to");
    adjList[from].transitions.push_back({to,input});
}


void Graph::dfs(unordered_set<int> vis, int cur_node) {
    if(vis.find(cur_node) != vis.end()) return;
    vis.insert(cur_node);
    adjList[cur_node].print();
    std::vector<int> neighbors = adjList[cur_node].getNeighbors();
    for(int i = 0; i < neighbors.size(); i++) {
        dfs(vis, neighbors[i]);
    }
}

unordered_map<int, node>* Graph:: get_nodes() {
    return &adjList;
}
