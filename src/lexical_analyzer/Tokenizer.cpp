#include "lexical_analyzer/Tokenizer.hpp"


#include "lexical_analyzer/DFA_Builder.hpp"
#include "lexical_analyzer/DFA_Minimizer.hpp"
#include "Constants.hpp"

#include <chrono>
#include <fstream>
#include <ratio>

Tokenizer::Tokenizer(NFA* nfa) {
  std::chrono::high_resolution_clock::time_point t1, t2;
  std::chrono::duration<double> time_span;

  printf("Converting NFA to DFA...\n");

  t1 = std::chrono::high_resolution_clock::now();
  DFA_Builder builder(nfa);
  DFA* built_dfa = builder.get_DFA();

  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
  printf("Converted NFA successfully in %.3f sec\nMinimizing DFA...\n", time_span.count());
  t1 = std::chrono::high_resolution_clock::now();

  DFA_Minimizer* minimizer = new DFA_Minimizer(built_dfa);
  DFA* minimized_dfa = (*minimizer).get_minimal_DFA();
  delete minimizer;

  t2 = std::chrono::high_resolution_clock::now();
  time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
  printf("Minimization done successfully in %.3f sec\n", time_span.count());

  this->dfa = minimized_dfa;
}

Tokenizer::~Tokenizer() {
  delete dfa;
}
void Tokenizer::tokenize(string inputFile, string outputFile){
    ifstream in(inputFile);
    ofstream out(outputFile);

    string line;
    while(getline(in, line)){
        line += " ";
        string lastAcceptanceType;
        int lastAcceptanceIndex = -1;
        node currentNode = (*(*dfa->get_nodes()).get_nodes())[dfa->get_start_node()];

        for(int i = 0; i < (int) line.size(); ++i){
            if(isspace(line[i])){
                if(lastAcceptanceIndex == -1){
                        if(currentNode.id != dfa->get_start_node())
                            out <<"Unidentified token." <<endl;
                }
                else{
                    out <<lastAcceptanceType <<endl;
                    i = lastAcceptanceIndex;
                    lastAcceptanceIndex = -1;
                    lastAcceptanceType = "";
                }
                currentNode = (*(*dfa->get_nodes()).get_nodes())[dfa->get_start_node()];
            }
            else{
                int nxtNodeID = currentNode.makeTransition(string(1, line[i]));
                currentNode = (*(*dfa->get_nodes()).get_nodes())[nxtNodeID];
                if(currentNode.id == dfa->get_end_node()){
                    if(lastAcceptanceIndex == -1){
                        out <<"Unidentified token." <<endl;
                    }
                    else{
                        out <<lastAcceptanceType <<endl;
                        i = lastAcceptanceIndex;
                        lastAcceptanceIndex = -1;
                        lastAcceptanceType = "";
                    }
                    currentNode = (*(*dfa->get_nodes()).get_nodes())[dfa->get_start_node()];
                }
                else if(currentNode.isAcceptance()){
                    lastAcceptanceIndex = i;
                    lastAcceptanceType = currentNode.type;
                }
            }
        }
    }

    in.close();
    out.close();
}

void Tokenizer::tokenize(string inputFile){
    ifstream in(inputFile);

    tokens.clear();
    indexOfNextToken = 0;

    string line;
    while(getline(in, line)){
        line += " ";
        string lastAcceptanceType;
        int lastAcceptanceIndex = -1;
        node currentNode = (*(*dfa->get_nodes()).get_nodes())[dfa->get_start_node()];

        for(int i = 0; i < (int) line.size(); ++i){
            if(isspace(line[i])){
                if(lastAcceptanceIndex == -1){
                        if(currentNode.id != dfa->get_start_node())
                            tokens.push_back("Unidentified token.");
                }
                else{
                    tokens.push_back(lastAcceptanceType);
                    i = lastAcceptanceIndex;
                    lastAcceptanceIndex = -1;
                    lastAcceptanceType = "";
                }
                currentNode = (*(*dfa->get_nodes()).get_nodes())[dfa->get_start_node()];
            }
            else{
                int nxtNodeID = currentNode.makeTransition(string(1, line[i]));
                currentNode = (*(*dfa->get_nodes()).get_nodes())[nxtNodeID];
                if(currentNode.id == dfa->get_end_node()){
                    if(lastAcceptanceIndex == -1){
                        tokens.push_back("Unidentified token.");
                    }
                    else{
                        tokens.push_back(lastAcceptanceType);
                        i = lastAcceptanceIndex;
                        lastAcceptanceIndex = -1;
                        lastAcceptanceType = "";
                    }
                    currentNode = (*(*dfa->get_nodes()).get_nodes())[dfa->get_start_node()];
                }
                else if(currentNode.isAcceptance()){
                    lastAcceptanceIndex = i;
                    lastAcceptanceType = currentNode.type;
                }
            }
        }
    }
    in.close();
}

string Tokenizer::nextToken(){
    if(indexOfNextToken == (int)tokens.size())
        return Constants::END_OF_INPUT;
    return tokens[indexOfNextToken];
}

string Tokenizer::getNextToken() {
    if(indexOfNextToken == (int)tokens.size())
        return Constants::END_OF_INPUT;
    return tokens[indexOfNextToken++];
}
