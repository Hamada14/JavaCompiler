#include <fstream>
#include <iostream>

#include "Config.hpp"
#include "lexical_analyzer/LexicalRulesParser.hpp"
#include "lexical_analyzer/Tokenizer.hpp"

using namespace std;

Tokenizer* getLanguageTokenizer()
{
        std::ifstream input_file;
        LexicalRulesParser* lexical_rules_parser = new LexicalRulesParser(new LanguageSpecParser());
        input_file.open(Config::getInstance()->get(Config::LANGUAGE_SPEC_PATH_KEY));
        Tokenizer* tokenizer = lexical_rules_parser->getLanguageTokenizer(&input_file);
        input_file.close();
        return tokenizer;
}

int main(int argc, const char* argv[])
{
        Config* program_config = Config::getInstance();
        program_config->init(argc, argv);

        Tokenizer* tokenizer = getLanguageTokenizer();
        tokenizer->tokenize(program_config->get(Config::INPUT_PROGRAM_PATH_KEY),
              program_config->get(Config::TOKEN_OUTPUT_PATH_KEY));
}
