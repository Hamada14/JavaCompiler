#ifndef PRINT_HPP_INCLUDED
#define PRINT_HPP_INCLUDED

#include <climits>
#include <cstring>
#include <iostream>
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
    FILE* file;
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
    Print(int start_node, string file_name)
    {
        file = freopen(file_name.c_str(), "w", stdout);
        this->shift = start_node;
        print_horizntal_line();
        vector<string> head;
        head.push_back("Node"), head.push_back("Type"), head.push_back("Priority"),
            head.push_back("Acceptance");
        for (int c = 0; c < 128; ++c)
            head.push_back("\"" + int_to_string(c) + "\"");
        cout << vertical;
        for (string str : head)
        {
            str = handle_length(str);
            cout << str << vertical;
        }
        cout << endl;
        print_horizntal_line();
    }
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
