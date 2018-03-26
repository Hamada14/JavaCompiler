#ifndef TOKENIZER_H
#define TOKENIZER_H

using namespace std;

#include <iostream>
#include <stdio.h>
#include <string>


#include "DFA.hpp"
#include "NFA.hpp"

class Tokenizer
{
    private:
        int indexOfNextToken;
        vector<string> tokens;
        DFA* dfa;

    public:
        Tokenizer(NFA*);
        ~Tokenizer();
        void tokenize(string inputFile, string outputFile);
        void tokenize(string inputFile);
        string nextToken();
};

#endif // TOKENIZER_H
