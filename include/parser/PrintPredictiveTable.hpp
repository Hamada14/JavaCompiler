#ifndef PRINTPREDICITVETABLE_HPP_INCLUDED
#define PRINTPREDICITVETABLE_HPP_INCLUDED

#include <climits>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


class PrintPredictiveTable
{
private:
    int block_length = 60;
    char dash = '-';
    char pls = '+';
    char vertical = '|';
    int table_attributes;
    ofstream output_file;
    void print_horizntal_line();
    string handle_length(string str);
    string int_to_string(int num);

public:
    PrintPredictiveTable(string);
    void printHeader(std::vector<string> head);
    void printData(std::vector<string> data);
    void close_file();
};

#endif
