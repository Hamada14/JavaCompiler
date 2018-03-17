#ifndef TOKENIZER_H
#define TOKENIZER_H

#include "DFA.h"

using namespace std;
#include <iostream>
#include <stdio.h>
#include <string>

class Tokenizer
{
    public:
        void tokenize(string inputFile, string outputFile, DFA dfa);
};

#endif // TOKENIZER_H
