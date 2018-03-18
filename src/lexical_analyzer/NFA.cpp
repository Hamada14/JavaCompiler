#include "lexical_analyzer/NFA.hpp"


NFA::NFA(char ch) {
    start_node = g.add_node(false, "");
    end_node = g.add_node(false, "");
    g.add_edge(start_node, end_node, string(&ch));
}

NFA:: NFA(Graph &g, int start_node, int end_node, int priority) {
    this->g = g;
    this->start_node = start_node;
    this->end_node = end_node;
    this->set_priority(priority);
}

NFA::NFA() {}

NFA::~NFA() {
    
}

Graph* NFA:: get_graph() {
    return &g;
}
int NFA:: get_start_node() {
    return start_node;
}

int NFA:: get_end_node() {
    return end_node;
}

int NFA:: get_priority() {
    return (*g.get_nodes())[end_node].priority;
}
void NFA:: set_graph(const Graph &g) {
    this->g = g;
}
void NFA:: set_start_node(int node) {
    start_node = node;
}
void NFA:: set_end_node(int node) {
    end_node = node;
}
void NFA:: set_priority(int priority) {
    (*g.get_nodes())[end_node].priority = priority;
}

void NFA:: set_type(std::string type) {
    (*g.get_nodes())[end_node].type = type;
}

void NFA:: set_acceptnace(std::string type, int priority) {
    if(priority < get_priority())
        return;
    set_priority(priority);
    set_type(type);
}

NFA* NFA:: clone() {

    Graph g;

    unordered_map<int, node> *nodes = this->get_graph()->get_nodes();
    unordered_map<int, int> newId;

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);

    
    return new NFA(g,newId[start_node],newId[end_node],this->get_priority());
}

NFA* NFA::orOperation(NFA &nfa) {
    Graph g;
    int startNode = g.add_node(false, "");

    unordered_map<int, node> *nodes = this->get_graph()->get_nodes();
    unordered_map<int, int> newId;
    string type1 = (*nodes)[end_node].type;

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);


    nodes = nfa.get_graph()->get_nodes();

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);

    string type2 = (*nodes)[nfa.get_end_node()].type;

    g.add_edge(startNode, newId[this->start_node], "/L");
    g.add_edge(startNode, newId[nfa.get_start_node()], "/L");

    int endNode = g.add_node(false, "");
    
    g.add_edge(newId[this->end_node], endNode,"/L");
    g.add_edge(newId[nfa.get_end_node()], endNode,"/L");

    return new NFA(g,startNode,endNode);
}

NFA* NFA::concatenateOperation(NFA &nfa) {
    Graph g;
    int startNode = g.add_node(false, "");

    unordered_map<int, node> *nodes = this->get_graph()->get_nodes();
    unordered_map<int, int> newId;


    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);


    nodes = nfa.get_graph()->get_nodes();

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);


    g.add_edge(startNode, newId[this->start_node], "/L");


    int endNode = g.add_node(false, "");

    g.add_edge(newId[nfa.get_end_node()], endNode,"/L");

    return new NFA(g,startNode,endNode);
}


NFA* NFA::asteriskOperation() {
    Graph g;
    int startNode = g.add_node(false, "");

    unordered_map<int, node> *nodes = this->get_graph()->get_nodes();
    unordered_map<int, int> newId;

    int endNode = g.add_node(false, "");

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);

    g.add_edge(startNode, newId[start_node], "/L");
    g.add_edge(newId[end_node], endNode, "/L");
    g.add_edge(startNode, endNode, "/L");
    g.add_edge(endNode, startNode, "/L");

    return new NFA(g,startNode,endNode);
}


NFA* NFA::plusOperation() {
    Graph g;
    int startNode = g.add_node(false, "");

    unordered_map<int, node> *nodes = this->get_graph()->get_nodes();
    unordered_map<int, int> newId;

    int endNode = g.add_node(false, "");

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);

    g.add_edge(startNode, newId[start_node], "/L");
    g.add_edge(newId[end_node], endNode, "/L");
    g.add_edge(endNode, startNode, "/L");

    return new NFA(g,startNode,endNode);
}



