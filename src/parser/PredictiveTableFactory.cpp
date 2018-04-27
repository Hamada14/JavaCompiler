#include "parser/PredictiveTableFactory.hpp"

#include "parser/ParserRulesReader.hpp"

PredictiveTableFactory* PredictiveTableFactory::instance = NULL;

PredictiveTableFactory::PredictiveTableFactory(ParserRulesReader* reader) : reader(reader) {
}

PredictiveTableFactory::~PredictiveTableFactory() {
    delete reader;
};

PredictiveTableFactory* PredictiveTableFactory::getInstance() {
    if(instance == NULL) {
        instance = new PredictiveTableFactory(new ParserRulesReader());
    }
    return instance;
}

PredictiveTable PredictiveTableFactory::getTable(std::ifstream *input_file, std::ofstream *fixed_rules_output) {
    LL1Grammar ll1_grammar = reader->getLL1Grammar(input_file, fixed_rules_output);

    PredictiveTable table(ll1_grammar.rule_table, ll1_grammar.start_state);
    return table;
}
