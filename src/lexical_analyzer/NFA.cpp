#include "lexical_analyzer/NFA.hpp"

NFA::NFA(char ch) {
    start_node = g.add_node(false, "");
    end_node = g.add_node(false, "");
    string edge_input = "";
    edge_input += ch;
    if(ch == '\0') {
        edge_input = LAMBDA;
    }
    g.add_edge(start_node, end_node, edge_input);
}

NFA:: NFA(std::string str) {
    if(str.empty()) return;
    start_node = g.add_node(false, "");
    int end_node = start_node;
    for(char ch: str) {
        int cur = g.add_node(false, "");
        g.add_edge(end_node,cur,string(1,ch));
        end_node = cur;
    }
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

    Graph g;

    unordered_map<int, node> *nodes = this->get_graph()->get_nodes();
    unordered_map<int, int> newId;

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);


    return new NFA(g,newId[start_node],newId[end_node]);
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

    g.add_edge(startNode, newId[this->start_node], LAMBDA);
    g.add_edge(startNode, newId[nfa.get_start_node()], LAMBDA);

    int endNode = g.add_node(false, "");

    g.add_edge(newId[this->end_node], endNode,LAMBDA);
    g.add_edge(newId[nfa.get_end_node()], endNode,LAMBDA);

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

    g.add_edge(startNode, newId[this->start_node], LAMBDA);

    nodes = nfa.get_graph()->get_nodes();

    for(auto n:*nodes)
        newId[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

    for(auto n:*nodes)
        for(transition tr: n.second.transitions)
            g.add_edge(newId[n.second.id], newId[tr.next], tr.input);


    g.add_edge(newId[this->end_node], newId[nfa.start_node], LAMBDA);
    int endNode = g.add_node(false, "");

    g.add_edge(newId[nfa.get_end_node()], endNode,LAMBDA);

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

    g.add_edge(startNode, newId[start_node], LAMBDA);
    g.add_edge(newId[end_node], endNode, LAMBDA);
    g.add_edge(startNode, endNode, LAMBDA);
    g.add_edge(endNode, startNode, LAMBDA);

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

    g.add_edge(startNode, newId[start_node], LAMBDA);
    g.add_edge(newId[end_node], endNode, LAMBDA);
    g.add_edge(endNode, startNode, LAMBDA);

    return new NFA(g,startNode,endNode);
}

NFA* NFA ::combine(vector<NFA*> &nfas) {
    Graph g;
    int startNode = g.add_node(false, "");
    unordered_map<int ,int> new_id;
    vector<int> endNodes;
    for(NFA* nfa: nfas) {
        unordered_map<int, node> *nodes = nfa->get_graph()->get_nodes();
        for(auto n:*nodes)
            new_id[n.second.id] = g.add_node(n.second.acceptance, n.second.type, n.second.priority);

        for(auto n:*nodes)
            for(transition tr: n.second.transitions)
                g.add_edge(new_id[n.second.id], new_id[tr.next], tr.input);

        g.add_edge(startNode, new_id[nfa->get_start_node()], LAMBDA);
        endNodes.push_back(new_id[nfa->get_end_node()]);
    }
    int endNode = g.add_node(false, "");
    for(int x: endNodes)
        g.add_edge(x, endNode, LAMBDA);

    return new NFA(g,startNode,endNode);
}
