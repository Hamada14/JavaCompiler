#include "lexical_analyzer/NFA.hpp"

NFA::NFA(int ch) {
    if(ch == '\0')
        ch = lambda;
    start_node = g.add_node(false, "");
    end_node = g.add_node(false, "");
    g.add_edge(start_node, end_node, ch);
}

NFA:: NFA(Graph &g, int start_node, int end_node) {
    this->g = g;
    this->start_node = start_node;
    this->end_node = end_node;
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
    (*g.get_nodes())[end_node].acceptance = true;
}


void NFA:: set_acceptance(std::string type, int priority) {
    if(priority < get_priority())
        return;
    (*g.get_nodes())[end_node].acceptance = true;
    set_priority(priority);
    set_type(type);
}

NFA* NFA:: clone() {
    return new NFA(*this);
}

int get_new_id(int id, int shift) {
    return id+shift;
}

NFA* NFA::orOperation(NFA &nfa) {
    int s1 = 1, s2 = 1+this->get_graph()->size();
    Graph g;
    int startNode = g.add_node(false, "");

    vector<node> *nodes = this->get_graph()->get_nodes();
    string type1 = (*nodes)[end_node].type;

    for(node &n:*nodes)
        g.add_node(n.acceptance, n.type, n.priority);

    for(node &n:*nodes)
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: n.transitions[i])
                g.add_edge(get_new_id(n.id,s1), get_new_id(j,s1), i);



    nodes = nfa.get_graph()->get_nodes();

    for(node &n:*nodes)
        g.add_node(n.acceptance, n.type, n.priority);

    for(node &n:*nodes)
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: n.transitions[i])
                g.add_edge(get_new_id(n.id, s2), get_new_id(j,s2), i);


    string type2 = (*nodes)[nfa.get_end_node()].type;

    g.add_edge(startNode, get_new_id(this->start_node,s1), lambda);
    g.add_edge(startNode, get_new_id(nfa.get_start_node(), s2), lambda);

    int endNode = g.add_node(false, "");

    g.add_edge(get_new_id(this->end_node,s1), endNode,lambda);
    g.add_edge(get_new_id(nfa.get_end_node(), s2), endNode,lambda);

    return new NFA(g,startNode,endNode);
}


NFA* NFA::concatenateOperation(NFA &nfa) {

    int s1 = 1, s2 = 1+this->get_graph()->size();
    Graph g;
    int startNode = g.add_node(false, "");

    vector<node> *nodes = this->get_graph()->get_nodes();

    for(node &n:*nodes)
        g.add_node(n.acceptance, n.type, n.priority);

    for(node &n:*nodes)
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: n.transitions[i])
                g.add_edge(get_new_id(n.id,s1), get_new_id(j,s1), i);

    g.add_edge(startNode, get_new_id(this->start_node,s1), lambda);

    nodes = nfa.get_graph()->get_nodes();

    for(node &n:*nodes)
        g.add_node(n.acceptance, n.type, n.priority);
    for(node &n:*nodes)
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: n.transitions[i])
                g.add_edge(get_new_id(n.id,s2), get_new_id(j,s2), i);



    g.add_edge(get_new_id(this->end_node,s1), get_new_id(nfa.get_start_node(),s2), lambda);
    int endNode = g.add_node(false, "");

    g.add_edge(get_new_id(nfa.get_end_node(),s2), endNode,lambda);

    return new NFA(g,startNode,endNode);
}


NFA* NFA::asteriskOperation() {
    int s1 = 1;

    Graph g;
    int startNode = g.add_node(false, "");

    vector<node> *nodes = this->get_graph()->get_nodes();


    for(node &n:*nodes)
        g.add_node(n.acceptance, n.type, n.priority);

    for(node &n:*nodes)
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: n.transitions[i])
                g.add_edge(get_new_id(n.id,s1), get_new_id(j,s1), i);

    int endNode = g.add_node(false, "");
    g.add_edge(startNode, get_new_id(startNode, s1), lambda);
    g.add_edge(get_new_id(end_node, s1), endNode, lambda);
    g.add_edge(startNode, endNode, lambda);
    g.add_edge(endNode, startNode, lambda);

    return new NFA(g,startNode,endNode);
}


NFA* NFA::plusOperation() {

    int s1 = 1;
    Graph g;
    int startNode = g.add_node(false, "");

    vector<node> *nodes = this->get_graph()->get_nodes();


    for(node &n:*nodes)
        g.add_node(n.acceptance, n.type, n.priority);

    for(node &n:*nodes)
        for(int i = 0; i < MAX_INPUT_SIZE ; ++i)
            for(int j: n.transitions[i])
                g.add_edge(get_new_id(n.id,s1), get_new_id(j,s1), i);

    int endNode = g.add_node(false, "");
    g.add_edge(startNode, get_new_id(startNode, s1), lambda);
    g.add_edge(get_new_id(end_node, s1), endNode, lambda);
    g.add_edge(endNode, startNode, lambda);

    return new NFA(g,startNode,endNode);
}
