#include "Tokenizer.h"

void tokenize(string inputFile, string outputFile, DFA dfa){
    freopen(inputFile.c_str(), "r", stdin);
    freopen(outputFile.c_str(), "w", stdout);

    string line;
    while(getline(cin, line)){
        string lastAcceptanceType;
        int lastAcceptanceIndex = -1;
        node currentNode = (*(*dfa.get_nodes()).get_nodes())[dfa.get_start_node()];

        for(int i = 0; i < (int) line.size(); ++i){
            if(isspace(line[i])){
                if(lastAcceptanceIndex == -1){
                    if(currentNode.id != dfa.get_start_node())
                        cout <<"Unidentified token." <<endl;
                }
                else{
                    cout <<lastAcceptanceType <<endl;
                    i = lastAcceptanceIndex;
                    lastAcceptanceIndex = -1;
                    lastAcceptanceType = "";
                }
                currentNode = (*(*dfa.get_nodes()).get_nodes())[dfa.get_start_node()];
            }
            else{
                int nxtNodeID = currentNode.makeTransition(&"" [ line[i]]);
                currentNode = (*(*dfa.get_nodes()).get_nodes())[nxtNodeID];
                if(currentNode.id == dfa.get_end_node()){
                    if(lastAcceptanceIndex == -1){
                        cout <<"Unidentified token." <<endl;
                        currentNode = (*(*dfa.get_nodes()).get_nodes())[dfa.get_start_node()];
                    }
                    else{
                        cout <<lastAcceptanceType <<endl;
                        i = lastAcceptanceIndex;
                        lastAcceptanceIndex = -1;
                        lastAcceptanceType = "";
                    }
                }
                else if(currentNode.isAcceptance()){
                    lastAcceptanceIndex = i;
                    lastAcceptanceType = currentNode.type;
                }
            }
        }
    }
}
