#include "parser/PrintPredictiveTable.hpp"

#include "Util.hpp"

PrintPredictiveTable::PrintPredictiveTable(string file_name)
{
        this->output_file.open(file_name);
}

void PrintPredictiveTable::printHeader(std::vector<string> head) {
        this->table_attributes = head.size();
        print_horizntal_line();
        output_file << vertical;
        for (string str : head)
        {
                str = handle_length(str);
                output_file << str << vertical;
        }
        output_file << endl;
        print_horizntal_line();
}

void PrintPredictiveTable::print_horizntal_line()
{
        output_file << pls;
        for (int i = 0; i < this->table_attributes; ++i)
        {
                for (int j = 0; j < block_length; ++j)
                        output_file << dash;
                output_file << pls;
        }
        output_file << endl;
}

string PrintPredictiveTable::handle_length(string str)
{
        str = " " + str;
        while ((int)str.size() < block_length)
                str += ' ';
        return str;
}


void PrintPredictiveTable::printData(std::vector<string> data)
{
        output_file << vertical;
        for (string& nxt : data)
        {
                string str = handle_length(nxt);
                output_file << str << vertical;
        }
        output_file << endl;
        print_horizntal_line();
}

void PrintPredictiveTable::close_file()
{
        this->output_file.close();
}
