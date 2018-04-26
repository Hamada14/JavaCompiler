#include <fstream>
#include <iostream>

#include "..\include\Config.hpp"
#include "..\include\lexical_analyzer/LexicalRulesParser.hpp"
#include "..\include\lexical_analyzer/Tokenizer.hpp"

#include "..\include\parser/PredictiveTableFactory.hpp"

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

PredictiveTable getTable() {
    std::ifstream parse_rules_file;
    parse_rules_file.open(Config::getInstance()->get(Config::PARSING_RULES_PATH_KEY));
    std::ofstream ll1_grammar_file;
    ll1_grammar_file.open(Config::getInstance()->get(Config::LL1_GRAMMAR_PATH_KEY));
    PredictiveTable predictive_table = PredictiveTableFactory::getInstance()->getTable(&parse_rules_file,
                                                                                       &ll1_grammar_file
                                                                                      );

    return predictive_table;
}

int main(int argc, const char* argv[])
{
        Config* program_config = Config::getInstance();
        program_config->init(argc, argv);

        Tokenizer* tokenizer = getLanguageTokenizer();
        tokenizer->tokenize(program_config->get(Config::INPUT_PROGRAM_PATH_KEY),
                            program_config->get(Config::TOKEN_OUTPUT_PATH_KEY)
                          );

        PredictiveTable table = getTable();
}
