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
        printf("add_edge: Invalid node {%d, %d}.", from, to);
    adjList[from].transitions.push_back({to,input});
    adjList[from].input_to_node_map[input].push_back(to);
}

vector<int> Graph::get_nodes_of_transitions(int node, string input) {
    return adjList[node].input_to_node_map[input];
}


void Graph::dfs(unordered_set<int>& vis, int cur_node) {
    if(vis.find(cur_node) != vis.end()) return;
    vis.insert(cur_node);
    adjList[cur_node].print();
    std::vector<int> neighbors = adjList[cur_node].getNeighbors();
    for(int neighbor : neighbors) {
        dfs(vis, neighbor);
    }
}

int Graph::get_size() {
    return adjList.size();
}

unordered_map<int, node>* Graph:: get_nodes() {
    return &adjList;
}
