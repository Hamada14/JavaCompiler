#ifndef Graph_hpp
#define Graph_hpp

#include <string>
#include <unordered_map>
#include <vector>
#include <stdio.h>

#define infi 1<<20
#define epsilon 128
#define MAX_INPUT_SIZE 129


using namespace std;

struct transition {
    int next; // node id;
    string input; // string because /L == lamda
};

#include <iostream>
#include <algorithm>
#include <vector>
#include <unordered_set>

// it can be pool design pattern for better performance
struct node {
    int id;
    bool acceptance;
    string type;
    int priority = -(infi);
    vector< vector<int> > transitions;

    node(int id, bool acceptance, string type) :id(id), acceptance(acceptance), type(type){
        transitions = vector< vector<int> > (MAX_INPUT_SIZE);
    };
    node(int id, bool acceptance, string type, int priority) :id(id), acceptance(acceptance), type(type), priority(priority){
        transitions = vector< vector<int> > (MAX_INPUT_SIZE);
    };
    node(){
        transitions = vector< vector<int> > (MAX_INPUT_SIZE);
    };

    bool isAcceptance(){
        return acceptance;
    }

    void print() {
        printf("Current Node=>%d\n", id);
        if(acceptance) {
            std::cout << "Type=>" << type << ", priority=> " << priority << std::endl;
        }
        std::cout << "Neighbors => ";
       
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i) {
            for(int j: transitions[i])
                std:: cout << "{" << j << ", /" << i << "/" << "}, ";
        }
        std::cout << endl;
    }

    std::vector<int> getNeighbors() {
        std::vector<int> res;
    
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: transitions[i])
                res.push_back(j);
        
        std::sort( res.begin(), res.end() );
        res.erase( std::unique( res.begin(), res.end() ), res.end() );
        return res;
    }

    int makeTransition(int s){
        if(s < 0 || s >= MAX_INPUT_SIZE) {
            cerr << "Invalid input.\n" ;
            return -1;
        }
        if(transitions[s].size() > 1) {
            cerr << "there are more than one transition.\n";
            return -1;
        }
        return transitions[s][0];
    }

};

class Graph {

    int numberOfNodes = 0;
    vector<node> adjList;

    public:
        void dfs(unordered_set<int>&, int);
        int add_node(bool acceptance, string type);  // returns node id
        int add_node(bool acceptance, string type, int priority);  // returns node id
        void add_edge(int from, int to, int input); //function parameters(from, to) are node ids
        vector<int>* get_nodes_of_transitions(int node_id, int input);
        vector<node>* get_nodes();
        int size();
};



#endif /* Graph_hpp */
