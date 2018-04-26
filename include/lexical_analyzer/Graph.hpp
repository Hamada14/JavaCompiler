#ifndef Graph_hpp
#define Graph_hpp

#include <string>
#include <unordered_map>
#include <vector>
#include <stdio.h>

#include "Constants.hpp"

#define infi 1<<20

using namespace std;
using namespace Constants;

struct transition {
    int next; // node id;
    string input; // string because /L == lamda
};

#include <iostream>
#include <algorithm>
#include <unordered_set>

// it can be pool design pattern for better performance
struct node {
    int id;
    bool acceptance;
    string type;
    int priority = -(infi);
    vector< transition > transitions;
    unordered_map<string, vector<int> > input_to_node_map;

    node(int id, bool acceptance, string type) :id(id), acceptance(acceptance), type(type){};
    node(int id, bool acceptance, string type, int priority) :id(id), acceptance(acceptance), type(type), priority(priority){};
    node(){};

    bool isAcceptance(){
        return acceptance;
    }

    void print() {
        printf("Current Node=>%d\n", id);
        if(acceptance) {
            std::cout << "Type=>" << type << ", priority=> " << priority << std::endl;
        }
        std::cout << "Neighbors => ";
        for(transition trans : transitions) {
            cout << "{" << trans.next << ", /" << trans.input << "/" << "}, ";
        }
        std::cout << endl;
    }

    std::vector<int> getNeighbors() {
        std::vector<int> res;
        for(transition trans : transitions) {
            res.push_back(trans.next);
        }
        std::sort( res.begin(), res.end() );
        res.erase( std::unique( res.begin(), res.end() ), res.end() );
        return res;
    }

    int makeTransition(string s){
        if((int)input_to_node_map[s].size() == 1) {
          return input_to_node_map[s][0];
        }
        return -1;
    }
};

class Graph {

    static int numberOfNodes;
    unordered_map<int, node> adjList;

    public:
        void dfs(unordered_set<int>&, int);
        int add_node(bool acceptance, string type);  // returns node id
        int add_node(bool acceptance, string type, int priority);  // returns node id
        void add_edge(int from, int to, string input); //function parameters(from, to) are node ids
        int get_size();
        vector<int> get_nodes_of_transitions(int, string);
        unordered_map<int, node>* get_nodes();

};



#endif /* Graph_hpp */
