#include "lexical_analyzer/ConfigParser.hpp"
#include <fstream>
#include <iostream>
#include "lexical_analyzer/NFA.hpp"
#include "DFA_Builder.hpp"
#include "DFA_Minimizer.hpp"

using namespace std;

NFA* readLanguageSpecs() {
    std::ifstream input_file;
    ConfigParser* config_parser = new ConfigParser(new LanguageSpecParser());
    std::string config_file_path = config_parser->getConfigFilePath();
    input_file.open(config_file_path);
    return config_parser->readLanguageSpec(&input_file);
}

void trace(DFA *dfa) {
    cout <<"Start node: " <<(*dfa).get_start_node() <<"\nEnd node: " <<(*dfa).get_end_node() <<endl;
    Graph *g = (*dfa).get_nodes();
    auto adjList = (*g).get_nodes();
    cout <<"Number of nodes: " <<(*adjList).size() <<endl;
    for(auto p : *adjList) p.second.print();
}


int main(int argc, const char * argv[]) {
    freopen("output.txt", "w", stdout);
    NFA* language_nfa = readLanguageSpecs();

    DFA_Builder *builder = new DFA_Builder(language_nfa);
    DFA* dfa = (*builder).get_DFA();
    DFA_Minimizer *minimizer = new DFA_Minimizer(dfa);
    DFA* minimized_dfa = (*minimizer).get_minimized_DFA();

    trace(dfa);
    trace(minimized_dfa);
}
