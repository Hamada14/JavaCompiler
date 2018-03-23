#include "lexical_analyzer/Print.hpp"

void Print::print_horizntal_line()
{
    cout << pls;
    for (int i = 0; i < table_attributs; ++i)
    {
        for (int j = 0; j < block_length; ++j)
            cout << dash;
        cout << pls;
    }
    cout << endl;
}

string Print::handle_length(string str)
{
    str = " " + str;
    while (str.size() < block_length)
        str += ' ';
    return str;
}

string Print::int_to_string(int num)
{
    stringstream ss;
    ss << num;
    string ret;
    ss >> ret;
    return ret;
}

void Print::pirnt_data(int id, string type, int prio, bool acc, vector<int> data)
{
    cout << vertical;
    string id_ = handle_length(int_to_string(id - shift + 1)), type_ = handle_length(type),
           prio_ = handle_length(int_to_string(prio));
    string acc_ = (acc ? "Acc" : "Non-Acc");
    acc_ = handle_length(acc_);
    cout << id_ << vertical << type_ << vertical << prio_ << vertical << acc_ << vertical;
    for (int nxt : data)
    {
        string str = handle_length(int_to_string(nxt - shift + 1));
        cout << str << vertical;
    }
    cout << endl;
    print_horizntal_line();
}

void Print::close_file()
{
    fclose(file);
}
