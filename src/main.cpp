#include "lexical_analyzer/ConfigParser.hpp"
#include <fstream>
#include <iostream>
#include "lexical_analyzer/NFA.hpp"
#include "lexical_analyzer/DFA_Builder.hpp"
#include "lexical_analyzer/DFA_Minimizer.hpp"
#include "lexical_analyzer/Tokenizer.hpp"
#include "lexical_analyzer/DFA.hpp"

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
    std::cout << "Building NFA..." << std::endl;
    clock_t begin = clock();

    NFA* language_nfa = readLanguageSpecs();

    printf("Built NFA successfully in %.3f sec\nConverting NFA to DFA...\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
    begin = clock();

    DFA_Builder *builder = new DFA_Builder(language_nfa);
    DFA* dfa = (*builder).get_DFA();

    printf("Converted NFA successfully in %.3f sec\nMinimizing DFA...\n", (double)(clock() - begin) / CLOCKS_PER_SEC);
    begin = clock();
    DFA_Minimizer *minimizer = new DFA_Minimizer(dfa);
    DFA* minimized_dfa = (*minimizer).get_minimal_DFA();

    printf("Minimization done successfully in %.3f sec\n", (double)(clock() - begin) / CLOCKS_PER_SEC);

    Tokenizer tokenizer;
    tokenizer.tokenize("tokens.txt", "out.txt", minimized_dfa);
}
