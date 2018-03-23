#ifndef PRINT_HPP_INCLUDED
#define PRINT_HPP_INCLUDED

#include <climits>
#include <cstring>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;


class Print
{
private:
    /*
        fixed length of each block
    */
    int block_length = 12;
    /*
        number of attrbiutes in table
    */
    int table_attributs = 132;
    /*
        shift id of nodes
    */
    int shift;
    /*
        dash symbol
    */
    char dash = '-';
    /*
        plus symbol
    */
    char pls = '+';
    /*
        vertical symbol
    */
    char vertical = '|';
    /*
        file to write table
    */
    ofstream output_file;
    /*
        print horizontal line of table
    */
    void print_horizntal_line();
    /*
        make string of fixes length
    */
    string handle_length(string str);
    /*
        convert integer to string
    */
    string int_to_string(int num);

public:
    /*
        constructor of the class
    */
    Print(int, string);
    /*
      Prints the table header to the output file specified in the constructor.
    */
    void printHeader();
    /*
        print data of one node
    */
    void pirnt_data(int id, string type, int prio, bool acc, vector<int> data);
    /*
        close file
    */
    void close_file();
};

#endif // PRINT_HPP_INCLUDED
