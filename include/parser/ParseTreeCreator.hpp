#ifndef PARSETREECREATOR_HPP_INCLUDED
#define PARSETREECREATOR_HPP_INCLUDED

#include <fstream>
#include <iostream>
#include <climits>
#include <cstring>
#include <iostream>
#include <map>
#include <stack>
#include <stdio.h>
#include <vector>

#include "lexical_analyzer/Tokenizer.hpp"
#include "parser/PredictiveTable.hpp"
#include "parser/RuleToken.hpp"

class ParseTreeCreator {
public:
 	ParseTreeCreator(std::string start_state, Tokenizer* tokens, PredictiveTable* predictive_table);
 	void createTable(std::ofstream* output_file, std::ofstream* parse_errors_file);
private:
	stack<RuleToken> parse_tree_stack;
	vector<RuleToken> print_terminal;
	deque<RuleToken> print_non_terminal;
	bool error;
	string start_state;
	Tokenizer* tokens;
	PredictiveTable* predictive_table;
	void substituteNonTerminal(vector<RuleToken> &new_rules);
	void print(std::ofstream* output_file);
	void handleNonTerminal(RuleToken top_of_stack, std::ofstream* output_file, std::ofstream* parse_errors_file);
};

#endif
