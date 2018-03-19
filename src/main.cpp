#include "lexical_analyzer/ConfigParser.hpp"
#include <fstream>
#include <iostream>
#include "lexical_analyzer/NFA.hpp"

NFA* readLanguaeSpecs() {
    std::ifstream input_file;
    ConfigParser* config_parser = new ConfigParser(new LanguageSpecParser());
    std::string config_file_path = config_parser->getConfigFilePath();
    input_file.open(config_file_path);
    return config_parser->readLanguageSpec(&input_file);
}

int main(int argc, const char * argv[]) {
    NFA* language_nfa = readLanguaeSpecs();
    return 0;
}
