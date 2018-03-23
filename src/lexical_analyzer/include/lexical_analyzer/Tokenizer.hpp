#ifndef TOKENIZER_H
#define TOKENIZER_H

using namespace std;

#include "DFA.hpp"
#include <iostream>
#include <stdio.h>
#include <string>

class Tokenizer
{
    public:
        void tokenize(string inputFile, string outputFile, DFA *dfa);
};

#endif // TOKENIZER_H
