#include "parser/ParserRulesReader.hpp"

#include <cstdlib>
#include <iostream>
#include <utility>

#include "Util.hpp"

const std::string ParserRulesReader::INVALID_INPUT_FILE_MESSAGE = "Parsing Rules input file is invalid.";

int ParserRulesReader::generated_state_id = 0;

ParserRulesReader::ParserRulesReader() {
}

ParserRulesReader::~ParserRulesReader() {
}


std::map<std::string, std::vector<ProductionRule> > ParserRulesReader::getLL1Grammar(std::ifstream* input_file,
                                                                                     std::ofstream* modified_rules) {
    if (!input_file->is_open()) {
        std::cerr << INVALID_INPUT_FILE_MESSAGE << std::endl;
        exit(0);
    }
    auto rule_table = readRules(input_file);
    eliminateLeftRecursion(rule_table);
    leftFactorGrammar(rule_table);
    printRulesTable(rule_table, *modified_rules);
    return rule_table;
}

std::map<std::string, std::vector<ProductionRule> > ParserRulesReader::readRules(std::ifstream* input) {
    std::vector<std::string> file_lines = readFile(input);
    std::set<std::string> rules_identifier = extractRuleIdentifiers(file_lines);
    return parseRules(rules_identifier, file_lines);
}

void ParserRulesReader::eliminateLeftRecursion(std::map<std::string, std::vector<ProductionRule> >& rule_table) {
    std::vector< std::vector<ProductionRule> > newly_added_states;
    int new_state_counter = 0;
    for(auto it_i = rule_table.begin(); it_i != rule_table.end(); it_i++) {
        for(auto it_j = rule_table.begin(); it_j != it_i; it_j++) {
            std::vector<ProductionRule> current_productions = it_i->second;
            std::vector<ProductionRule> new_rules;
            for(ProductionRule prod_rule : current_productions) {
                if(prod_rule.canLeftImmedSubstitute(it_j->first)) {
                    std::vector<ProductionRule> substituted_rules = prod_rule.leftImmedSubstitute(it_j->second);
                    new_rules.insert(std::end(new_rules), std::begin(substituted_rules), std::end(substituted_rules));
                } else {
                    new_rules.push_back(prod_rule);
                }
            }
            it_i->second = new_rules;
        }
        std::vector<ProductionRule> new_state_rules = leftImmedEliminate(it_i->second, it_i->first, new_state_counter);
        if(new_state_rules.size() != 0) {
            new_state_counter++;
            newly_added_states.push_back(new_state_rules);
        }
    }
    for(size_t i = 0; i < newly_added_states.size(); i++) {
        rule_table[ParserRulesReader::createStateName(i)] = newly_added_states[i];
    }
}

std::vector<ProductionRule> ParserRulesReader::leftImmedEliminate(std::vector<ProductionRule>& productions,
                                                                  std::string state_name, int new_state) {
    std::vector<ProductionRule> no_immediate_recursion;
    std::vector<ProductionRule> immediate_recursion;
    for(ProductionRule prod_rule : productions) {
        if(prod_rule.canLeftImmedSubstitute(state_name)) {
            immediate_recursion.push_back(prod_rule);
        } else {
            no_immediate_recursion.push_back(prod_rule);
        }
    }
    if(immediate_recursion.size() != 0 && no_immediate_recursion.size() == 0) {
        std::cerr << "Can't Eliminate left recursion in the Grammar.\nProgram is exiting..." << std::endl;
        exit(0);
    } else if(immediate_recursion.size() == 0) {
        return std::vector<ProductionRule>();
    }
    std::vector<ProductionRule> new_rules;
    for(ProductionRule rule : immediate_recursion) {
        std::vector<RuleToken> tokens = rule.getTokens();
        tokens.erase(tokens.begin());
        tokens.push_back(RuleToken(ParserRulesReader::createStateName(new_state)));
        new_rules.push_back(ProductionRule(tokens));
    }
    new_rules.push_back(ProductionRule({RuleToken(RuleToken::LAMBDA_VALUE)}));
    productions.clear();
    for(ProductionRule rule : no_immediate_recursion) {
        std::vector<RuleToken> tokens = rule.getTokens();
        tokens.push_back(RuleToken(ParserRulesReader::createStateName(new_state)));
        productions.push_back(ProductionRule(tokens));
    }
    return new_rules;
}

void ParserRulesReader::leftFactorGrammar(std::map<std::string, std::vector<ProductionRule> >& rule_table) {
    std::vector<std::string> to_be_factored_rules;
    for(auto rule_it : rule_table) {
        to_be_factored_rules.push_back(rule_it.first);
    }
    for(std::string rule : to_be_factored_rules) {
        leftFactorProduction(rule, rule_table);
    }
}

void ParserRulesReader::leftFactorProduction(std::string rule_name, std::map<std::string, std::vector<ProductionRule> >& rule_table) {
    std::vector<ProductionRule> to_be_factored_rules = rule_table[rule_name];
    std::map<RuleToken, std::vector<ProductionRule> > production_rule_prefix_map;
    std::vector<ProductionRule> new_rules;
    for(ProductionRule prod_rule : to_be_factored_rules) {
        if(prod_rule.getTokenCount() != 0) {
            production_rule_prefix_map[prod_rule.getToken(0)].push_back(prod_rule);
        } else {
            new_rules.push_back(ProductionRule({RuleToken(RuleToken::LAMBDA_VALUE)}));
        }
    }
    for(auto production_rule_prefix_it : production_rule_prefix_map) {
        if(production_rule_prefix_it.second.size() == 1) {
            new_rules.push_back(production_rule_prefix_it.second[0]);
            continue;
        }
        size_t prefix_length = ProductionRule::getCommonPrefixTokenCount(production_rule_prefix_it.second);
        std::vector<RuleToken> prefix_tokens = production_rule_prefix_it.second[0].getPrefixTokens(prefix_length);
        std::string new_state_name = ParserRulesReader::generateState();
        prefix_tokens.push_back(RuleToken(new_state_name));
        new_rules.push_back(ProductionRule(prefix_tokens));
        for(ProductionRule& prod_rule : production_rule_prefix_it.second) {
            prod_rule.popTokens(prefix_length);
        }
        rule_table[new_state_name] = production_rule_prefix_it.second;
        leftFactorProduction(new_state_name, rule_table);
    }
    rule_table[rule_name] = new_rules;
}

std::map<std::string, std::vector<ProductionRule> > ParserRulesReader::parseRules(std::set<std::string> rule_ids,
                                                                                  std::vector<std::string> input) {
    std::map<std::string, std::vector<ProductionRule> > rules;
    std::string current_id;
    for(size_t cur_line = 0; cur_line < input.size();) {
        current_id = ProductionRule::getIdentifier(input[cur_line]);
        std::string full_def = input[cur_line].substr(input[cur_line].find(ProductionRule::RULE_DEFINITION_OPERATOR) +
                                                      1);
        int next_line = cur_line + 1;
        while((size_t)next_line < input.size() && !ProductionRule::isRuleDefinition(input[next_line])) {
            full_def += input[next_line];
            next_line++;
        }
        std::vector<std::string> raw_rules = Util::splitQuoteSensitive(full_def, ProductionRule::RULES_SEPARATOR);
        for(std::string raw_rule : raw_rules) {
            if(Util::isEmptyString(raw_rule)) {
                continue;
            }
            ProductionRule rule(raw_rule, rule_ids);
            if(rule.isInvalid()) {
                std::cerr << "Invalid Rule specified in the declaration of {" << current_id << "}->{" << raw_rule <<
                    "}" << std::endl;
                exit(-1);
            }
            rules[current_id].push_back(rule);
        }
        cur_line = next_line;
    }
    return rules;
}

std::vector<std::string> ParserRulesReader::readFile(std::ifstream* input_file) {
    std::vector<std::string> file_lines;
    for (std::string line; getline(*input_file, line);) {
        file_lines.push_back(line);
    }
    return file_lines;
}

std::set<std::string> ParserRulesReader::extractRuleIdentifiers(std::vector<std::string>& input) {
    std::set<std::string> identifiers;
    for(std::string line : input) {
        if(ProductionRule::isRuleDefinition(line)) {
            identifiers.insert(ProductionRule::getIdentifier(line));
        }
    }
    return identifiers;
}


void ParserRulesReader::printRulesTable(std::map<std::string, std::vector<ProductionRule> > rule_table,
                                        std::ostream& o_stream) {
    for(auto it = rule_table.begin(); it != rule_table.end(); it++) {
        o_stream << ProductionRule::RULE_DEFINITION_START_MARKER << Util::WHITE_SPACE << it->first << Util::WHITE_SPACE;
        o_stream << ProductionRule::RULE_DEFINITION_OPERATOR << Util::WHITE_SPACE;
        std::vector<ProductionRule> rules = it->second;
        for(size_t i = 0; i < rules.size(); i++) {
            std::vector<RuleToken> tokens = rules[i].getTokens();
            for(RuleToken token : tokens) {
                o_stream << Util::WHITE_SPACE;
                if(token.getType() == RuleTokenType::TERMINAL) {
                    o_stream << Util::SINGLE_QUOTE << token.getValue()  << Util::SINGLE_QUOTE;
                } else {
                    o_stream << token.getValue();
                }
            }
            if(i != rules.size() - 1) {
                o_stream << Util::WHITE_SPACE << ProductionRule::RULES_SEPARATOR;
            }
        }
        o_stream << std::endl;
    }
}

std::string ParserRulesReader::createStateName(int id) {
    std::string str_id = Util::int_to_string(id);
    return "State_" + str_id;
}

std::string ParserRulesReader::generateState() {
    int id = generated_state_id++;
    std::string str_id = Util::int_to_string(id);
    return "Generated_State_" + str_id;
}
