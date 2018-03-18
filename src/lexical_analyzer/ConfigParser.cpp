#include "lexical_analyzer/ConfigParser.hpp"

const std::string ConfigParser::CONFIG_FILE_PATH = "input";

const char ConfigParser::DEFINITION_OPERATOR = ':';
const char ConfigParser::EXPRESSION_OPERATOR = '=';

const std::regex ConfigParser::REGULAR_EXPRESSION_REGEX("[a-zA-z][a-zA-z0-9]+\\s*\\=.*");
const std::regex ConfigParser::REGULAR_DEFINITION_REGEX("[a-zA-z][a-zA-z0-9]+\\s*\\:.*");
const std::regex ConfigParser::PUNCTUATION_REGEX("\\[.*\\]");
const std::regex ConfigParser::KEYWORDS_REGEX("\\{.*\\}");

ConfigParser::ConfigParser(LanguageSpecParser* lang_sp) : lang_sp(lang_sp) {
        this->regex_table = new RegularExpressionTable();
        this->definition_table = new RegularExpressionTable();
}

ConfigParser::~ConfigParser() {
        for(int i = 0; i < (int)keywords.size(); i++) {
                delete keywords[i];
        }
        for(int i = 0; i < (int)punctuations.size(); i++) {
                delete punctuations[i];
        }
        delete regex_table;
        delete lang_sp;
        delete definition_table;
}

std::string ConfigParser::getConfigFilePath() {
        return CONFIG_FILE_PATH;
}

void ConfigParser::readLanguageSpec(std::ifstream* input_file) {
        validateInputFile(input_file);
        readLanguage(input_file);
}

NFA* ConfigParser::readLanguage(std::ifstream* input_file) {
        std::string current_line;
        int priority = -1;
        while(getline(*input_file, current_line)) {
                parseLine(Util::trim(current_line), priority);
                priority--;
        }
        return getResultNFA();
}

NFA* ConfigParser::getResultNFA() {
        std::vector<NFA*> result_items = definition_table->getValues();
        NFA* result = result_items[0]->clone();
        for(int i = 1; i < (int)result_items.size(); i++) {
                NFA* new_result = result->orOperation(*result_items[i]);
                NFA* temp = result;
                result = new_result;
                delete temp;
        }
        for(int i = 0; i < (int)keywords.size(); i++) {
                NFA* new_result = result->orOperation(*keywords[i]);
                NFA* temp = result;
                result = new_result;
                delete temp;
        }
        for(int i = 1; i < (int)punctuations.size(); i++) {
                NFA* new_result = result->orOperation(*punctuations[i]);
                NFA* temp = result;
                result = new_result;
                delete temp;
        }
        return result;
}

void ConfigParser::parseLine(std::string current_line, int priority) {
        if(regex_match(current_line, REGULAR_EXPRESSION_REGEX)) {
                parseRegularExpression(current_line, priority);
        } else if(regex_match(current_line, REGULAR_DEFINITION_REGEX)) {
                parseRegularDefinition(current_line, priority);
        } else if(regex_match(current_line, PUNCTUATION_REGEX)) {
                parsePunctuation(current_line, priority);
        } else if(regex_match(current_line, KEYWORDS_REGEX)) {
                parseKeywords(current_line, priority);
        } else {
                std::cerr << "Error! Line doesn't follow the required description.\nProgram is exitting...";
                exit(0);
        }
}

void ConfigParser::parseRegularExpression(std::string current_line, int priority) {
        std::string operand_1, operand_2;
        disassembleExpression(current_line, EXPRESSION_OPERATOR, operand_1, operand_2);
        NFA* result = lang_sp->parseRegularExpression(operand_2, regex_table);
        result->set_priority(priority);
        regex_table->addExpressionNFA(operand_1, result);
}

void ConfigParser::parseRegularDefinition(std::string current_line, int priority) {
        std::string operand_1, operand_2;
        disassembleExpression(current_line, DEFINITION_OPERATOR, operand_1, operand_2);
        NFA* result = lang_sp->parseRegularExpression(operand_2, regex_table);
        result->set_priority(priority);
        definition_table->addExpressionNFA(operand_1, result);
}

void ConfigParser::parsePunctuation(std::string current_line, int priority) {
        std::vector<std::string> splitted_punctuations = Util::split(current_line.substr(1, current_line.length() - 2), ' ');
        for(std::string punc : splitted_punctuations) {
                if(!isValidPunctuation(punc)) {
                        std::cerr << "Error! Invalid punctuation specified {" << punc << "}\nProgram is exitting...";
                        exit(-1);
                }
                punctuations.push_back(keywordToNFA(punc.substr(punc.length() - 1), priority));
        }
}

void ConfigParser::parseKeywords(std::string current_line, int priority) {
        std::vector<std::string> splitted_keywords = Util::split(current_line.substr(1, current_line.length() - 2), ' ');
        for(std::string keyword : splitted_keywords) {
                if(!isValidKeywords(keyword)) {
                        std::cerr << "Error! Invalid keywords specified\nProgram is exitting...";
                        exit(-1);
                }
                keywords.push_back(keywordToNFA(keyword, priority));
        }
}

bool ConfigParser::isValidKeywords(std::string keyword) {
        for(int i = 0; i < (int)keyword.length(); i++) {
                if(lang_sp->isReservedSymbol(keyword[i]) || (keyword[i] == '\\' && i != (int)keyword.length() - 1
                                                             && !lang_sp->isReservedSymbol(keyword[i + 1])))
                        return false;
                if(keyword[i] == '\\') i++;
        }
        return true;
}

bool ConfigParser::isValidPunctuation(std::string punc) {
        if((punc.length() == 1 && !lang_sp->isReservedSymbol(punc[0])) ||
           (punc.length() == 2 && punc[0] == '\\' && lang_sp->isReservedSymbol(punc[1]))) {
                return true;
        }
        return false;
}

NFA* ConfigParser::keywordToNFA(std::string keyword, int priority) {
        NFA* result = new NFA(keyword[0]);
        for(int i = 1; i < (int)keyword.length(); i++) {
                NFA* temp = result;
                NFA* char_nfa = new NFA(keyword[i]);
                result = result->concatenateOperation(*char_nfa);
                delete temp;
                delete char_nfa;
        }
        result->set_priority(priority);
        result->set_type(keyword);
        return result;
}

void ConfigParser::disassembleExpression(std::string expression, char operator_, std::string& operand_1, std::string& operand_2) {
        int operator_index = (int)expression.find(operator_);
        operand_1 = Util::trim(expression.substr(0, operator_index));
        operand_2 = Util::trim(expression.substr(operator_index + 1));
}

void ConfigParser::validateInputFile(std::ifstream* input_file) {
        if(!*input_file) {
                std::cerr << "Couldn't read the input file.\nProgram is exitting...";
                exit(0);
        }
}
