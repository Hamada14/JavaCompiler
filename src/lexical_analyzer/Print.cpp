#include "..\..\include\lexical_analyzer/Print.hpp"

#include "..\..\include\Util.hpp"

#define INFI (1<<20)

Print::Print(int start_node, string file_name)
{
        this->shift = start_node;
        this->output_file.open(file_name);
}

void Print::printHeader() {
        print_horizntal_line();
        vector<string> head;
        head.push_back("Node"), head.push_back("Type"), head.push_back("Priority"),
        head.push_back("Acceptance");
        for (int c = 0; c < 128; ++c)
                head.push_back("\"" + Util::int_to_string(c) + "\"");
        output_file << vertical;
        for (string str : head)
        {
                str = handle_length(str);
                output_file << str << vertical;
        }
        output_file << endl;
        print_horizntal_line();
}

void Print::print_horizntal_line()
{
        output_file << pls;
        for (int i = 0; i < table_attributs; ++i)
        {
                for (int j = 0; j < block_length; ++j)
                        output_file << dash;
                output_file << pls;
        }
        output_file << endl;
}

string Print::handle_length(string str)
{
        str = " " + str;
        while ((int)str.size() < block_length)
                str += ' ';
        return str;
}



void Print::pirnt_data(int id, string type, int prio, bool acc, vector<int> data)
{
        output_file << vertical;
        string id_ = handle_length(Util::int_to_string(id - shift + 1)), type_ = handle_length(type),
        prio_ = prio > -(INFI) ? Util::int_to_string(prio) : "N/A";
        prio_ = handle_length(prio_);
        string acc_ = (acc ? "Acc" : "Non-Acc");
        acc_ = handle_length(acc_);
        output_file << id_ << vertical << type_ << vertical << prio_ << vertical << acc_ << vertical;
        for (int nxt : data)
        {
                string str = handle_length(Util::int_to_string(nxt - shift + 1));
                output_file << str << vertical;
        }
        output_file << endl;
        print_horizntal_line();
}

void Print::close_file()
{
        this->output_file.close();
}
