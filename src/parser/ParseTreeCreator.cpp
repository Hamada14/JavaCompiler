#include "parser/ParseTreeCreator.hpp"

#include "Util.hpp"

ParseTreeCreator::ParseTreeCreator(std::string start_state, Tokenizer* tokens, PredictiveTable* predictive_table)
: start_state(start_state), tokens(tokens), predictive_table(predictive_table) {
	error = false;
}

void ParseTreeCreator::createTable(std::ofstream* output_file) {
	RuleToken non_terminal(start_state), dollar(Constants::END_OF_INPUT);
	parse_tree_stack.push(dollar), parse_tree_stack.push(non_terminal);
	print_non_terminal.push_front(non_terminal);
	while (!parse_tree_stack.empty()) {
		RuleToken top_of_stack = parse_tree_stack.top();
        parse_tree_stack.pop();
		if (top_of_stack.getType() == RuleTokenType::END_OF_INPUT) {
			if (tokens->nextToken() != Constants::END_OF_INPUT)
				error = true;
            parse_tree_stack.push(top_of_stack);
            tokens->getNextToken();
			continue;
		} else if (top_of_stack.getType() == RuleTokenType::LAMBDA_TERMINAL) {
			continue;
		} else if (top_of_stack.getType() == RuleTokenType::NON_TERMINAL) {
            handleNonTerminal(top_of_stack, output_file);
		} else if (top_of_stack.getType() == RuleTokenType::TERMINAL) {
			if (top_of_stack.getValue() != tokens->nextToken())
				error = true;
			else
                tokens->getNextToken();
		}
	}
}

void ParseTreeCreator::handleNonTerminal(RuleToken top_of_stack, std::ofstream* output_file) {
	std::cout << "Fuck " << tokens->nextToken() << std::endl;
	std::cout << "ere " << (predictive_table->getTransitionType(top_of_stack.getValue(), tokens->nextToken()) == TransitionType::LEGAL) << std::endl;
    if (predictive_table->getTransitionType(top_of_stack.getValue(), tokens->nextToken()) == TransitionType::LEGAL) {
        vector<RuleToken> transitions = predictive_table->getTransition(top_of_stack.getValue(), tokens->nextToken());
        for (int i = transitions.size() - 1; i >= 0; --i)
            parse_tree_stack.push(transitions[i]);
        substituteNonTerminal(transitions);
        print(output_file);
    } else if (predictive_table->getTransitionType(top_of_stack.getValue(), tokens->nextToken()) == TransitionType::SYNC) {
        error = true;
    } else {
        error = true;
        parse_tree_stack.push(top_of_stack);
        tokens->getNextToken();
    }
}

void ParseTreeCreator::substituteNonTerminal(vector<RuleToken> &new_rules) {
    print_non_terminal.pop_front();
    for (int i = new_rules.size() - 1; i >= 0; --i)
        print_non_terminal.push_front(new_rules[i]);
    while (!print_non_terminal.empty() && print_non_terminal.front().getType() == RuleTokenType::TERMINAL)
        print_terminal.push_back(print_non_terminal.front()), print_non_terminal.pop_front();
}

void ParseTreeCreator::print(std::ofstream* output_file) {
    for (int i = 0; i < (int) print_terminal.size(); ++i)
        *output_file << print_terminal[i].getValue() << " ";
    for (int i = 0; i < (int) print_non_terminal.size(); ++i)
        *output_file << print_non_terminal[i].getValue() << " ";
    *output_file << std::endl;
}
