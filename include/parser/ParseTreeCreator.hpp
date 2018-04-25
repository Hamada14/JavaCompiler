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
 	ParseTreeCreator(std::string& start_state, Tokenizer tokens, PredictiveTable predictive_table);
 	void createTable(std::ifstream*, std::ofstream*);
private:
	stack<RuleToken> parse_tree_stack;
	string start_state;
	Tokenizer tokens;
	PredictiveTable predictive_table;
	bool error;
};

#endif
