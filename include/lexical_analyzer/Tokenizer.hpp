#ifndef TOKENIZER_H
#define TOKENIZER_H

using namespace std;

#include "DFA.hpp"
#include <iostream>
#include <stdio.h>
#include <string>

class Tokenizer
{
    private:
        int indexOfNextToken;
        vector<string> tokens;

    public:
        void tokenize(string inputFile, string outputFile, DFA *dfa);
        void tokenize(string inputFile, DFA *dfa);
        string nextToken();
};

#endif // TOKENIZER_H
