#ifndef Graph_hpp
#define Graph_hpp

#include <string>
#include <unordered_map>
#include <vector>

#define infi 1<<20

using namespace std;

struct transition {
    int next; // node id;
    string input; // string because /L == lamda
};


// it can be pool design pattern for better performance
struct node {
    int id;
    bool acceptance;
    string type;
    vector< transition > transitions;
    int priority = infi;
    node(int id, bool acceptance, string type) :id(id), acceptance(acceptance), type(type){};
    node(){};
    bool isAcceptance(){
        return acceptance;
    }

    int makeTransition(string s){
        for(transition t : transitions)
            if(t.input == s) return t.next;
        return -1;
    }

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
