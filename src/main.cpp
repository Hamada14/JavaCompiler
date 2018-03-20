#include "lexical_analyzer/ConfigParser.hpp"
#include <fstream>
#include <iostream>
#include "lexical_analyzer/NFA.hpp"
#include "DFA_Builder.hpp"

using namespace std;

NFA* readLanguageSpecs() {
    std::ifstream input_file;
    ConfigParser* config_parser = new ConfigParser(new LanguageSpecParser());
    std::string config_file_path = config_parser->getConfigFilePath();
    input_file.open(config_file_path);
    return config_parser->readLanguageSpec(&input_file);
}

int main(int argc, const char * argv[]) {
    NFA* language_nfa = readLanguageSpecs();
    cout <<(*language_nfa).get_start_node() <<endl;
    cout <<(*language_nfa).get_graph()->get_nodes()->size() <<endl;

    DFA_Builder *builder = new DFA_Builder(language_nfa);
    DFA* dfa = (*builder).get_DFA();

    cout <<(*dfa).get_start_node() <<" " <<(*dfa).get_end_node();
    Graph *g = (*dfa).get_nodes();
    auto adjList = (*g).get_nodes();
    cout <<"Number of nodes: " <<(*adjList).size() <<endl;
    for(auto p : *adjList) p.second.print();
}
