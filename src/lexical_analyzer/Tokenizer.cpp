#include "lexical_analyzer/Tokenizer.hpp"
#include <fstream>

void Tokenizer::tokenize(string inputFile, string outputFile, DFA *dfa){
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

void Tokenizer::tokenize(string inputFile, DFA *dfa){
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
        return nullptr;
    return tokens[indexOfNextToken++];
}
