#include "lexical_analyzer/Graph.hpp"
#include <stdio.h>


int Graph:: add_node(bool acceptance, string type) {
    int id = (int)adjList.size();
    adjList.push_back(node(id, acceptance, type));
    return id;
}

int Graph:: add_node(bool acceptance, string type, int priority) {
    int id = (int)adjList.size();;
    adjList.push_back(node(id, acceptance, type, priority));
    return id;
}

void Graph:: add_edge(int from, int to, int input) {
    if(from >= (int)adjList.size() || to >= (int)adjList.size())
        cerr << "add_edge: Invalid node.\n";
    adjList[from].transitions[input].push_back(to);
}

vector<int>* Graph::get_nodes_of_transitions(int node, int input) {
    return &adjList[node].transitions[input];
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

vector<node>* Graph:: get_nodes() {
    return &adjList;
}

int Graph:: size() {
    return (int)adjList.size();
}
