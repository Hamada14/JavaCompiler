#include "parser/ParseTreeCreator.hpp"

#include "Util.hpp"

ParseTreeCreator(std::string& start_state, Tokenizer tokens, PredictiveTable predictive_table)
: start_state(start_state), tokens(tokens), predictive_table(predictive_table) {
	error = false;
}

void ParseTreeCreator::createTable(std::ifstream*, std::ofstream*) {
	RuleToken non_terminal(start_state), cur_token_rule, top_of_stack, dollar("$");
	parse_tree_stack.push(dollar);
	parse_tree_stack.push(non_terminal);
	while (!parse_tree_stack.empty()) {
		top_of_stack = parse_tree_stack.top(), parse_tree_stack.pop();
		if (top_of_stack.getType() == RuleTokenType::DOLLAR_TERMINAL) {
			if (tokens.nextToken() != nullptr)
				error = true;
			break;
		} else if (top_of_stack.getType() == RuleTokenType::LAMBDA_TERMINAL) {
			continue;
		} else if (top_of_stack.getType() == RuleTokenType::NON_TERMINAL) {
			vector<RuleToken> transitions = predictive_table.getTransition(top_of_stack.getValue(), tokens.nextToken());
			for (int i = transitions.size() - 1; i >= 0; --i)
				parse_tree_stack.push(transitions[i]);
		} else if (top_of_stack.getType() == RuleTokenType::TERMINAL) {
			if (top_of_stack.getValue() != tokens.nextToken())
				error = true;
			tokens.getNextToken(); 
		}
	}
}