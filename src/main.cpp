#include <fstream>
#include <iostream>

#include "Config.hpp"
#include "lexical_analyzer/LexicalRulesParser.hpp"
#include "lexical_analyzer/Tokenizer.hpp"

#include "parser/PredictiveTableFactory.hpp"
#include "parser/ParseTreeCreator.hpp"

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

ParseTreeCreator getParseTreeCreator(Tokenizer* tokenizer) {
    std::ifstream parse_rules_file;
    parse_rules_file.open(Config::getInstance()->get(Config::PARSING_RULES_PATH_KEY));
    std::ofstream ll1_grammar_file;
    ll1_grammar_file.open(Config::getInstance()->get(Config::LL1_GRAMMAR_PATH_KEY));
    std::cout << "Creating Predictive Table " << std::endl;
    PredictiveTable* predictive_table = PredictiveTableFactory::getInstance()->getTable(&parse_rules_file,
                                                                                       &ll1_grammar_file
                                                                                      );
    std::cout << "Created Predictive Table successfully" << std::endl;
    std::string start_state = predictive_table->getStartState();
    return ParseTreeCreator(start_state, tokenizer, predictive_table);
}

int main(int argc, const char* argv[])
{
        Config* program_config = Config::getInstance();
        program_config->init(argc, argv);

        Tokenizer* tokenizer = getLanguageTokenizer();
        tokenizer->tokenize(program_config->get(Config::INPUT_PROGRAM_PATH_KEY),
                            program_config->get(Config::TOKEN_OUTPUT_PATH_KEY)
                          );
        tokenizer->tokenize(program_config->get(Config::INPUT_PROGRAM_PATH_KEY));

        ParseTreeCreator parse_tree_creator = getParseTreeCreator(tokenizer);
        std::ofstream parse_tree_output;
        parse_tree_output.open(Config::getInstance()->get(Config::PARSE_TREE_PATH_KEY));
        parse_tree_creator.createTable(&parse_tree_output);
        std::cout << "Program finished successfully" << std::endl;
}
