#ifndef STATE_HPP_INCLUDED
#define STATE_HPP_INCLUDED

#include "Graph.hpp"
#include <set>
#include <stdio.h>

/*
    this class holds a set of nodes that can be reduced to one node
    -- used in Building DFA to hold states which is built
    -- used in Minimizing DFA to hold partitions used in the algorithm
*/

class State
{
private:
    /*
        nodes of a certain state
    */
    set<int>* nodes;
    /*
        id of the state in the graph
    */
    int id;
    /*
        whether this node acceptance node or not
    */
    bool is_accepteance;
    /*
        type of reduced node
    */
    string type;
    /*
        priority of reduced node
    */
    int priority;

public:
    /*
        constructor of the class
    */
    State()
    {
        nodes = new set<int>;
    };
    /*
        getter of the nodes in the state
    */
    set<int>* get_nodes();
    /*
        getter of id of the state
    */
    int get_id();
    /*
        getter of priority of the state
    */
    int get_priority();
    /*
        getter of the acceptance of the state
    */
    bool get_acceptance();
    /*
        getter of the type of the state
    */
    string get_type();
    /*
        setter of the nodes in the state
    */
    void set_nodes(set<int>* n);
    /*
        add new node to this state
    */
    void insert_node(int n);
    /*
        setter of id of the state
    */
    void set_id(int idx);
    /*
        setter of priority of the state
    */
    void set_priority(int prio);
    /*
        setter of the acceptance of the state
    */
    void set_acceptance(bool s);
    /*
        setter of the type of the state
    */
    void set_type(string s);
};

#endif // STATE_HPP_INCLUDED
