#include "ConfigParser.hpp"

const std::string ConfigParser::CONFIG_FILE_PATH = "input";

const char ConfigParser::DEFINITION_OPERATOR = ':';
const char ConfigParser::EXPRESSION_OPERATOR = '=';

const std::regex ConfigParser::REGULAR_EXPRESSION_REGEX("[a-zA-z][a-zA-z0-9]+\\s*\\=.*");
const std::regex ConfigParser::REGULAR_DEFINITION_REGEX("[a-zA-z][a-zA-z0-9]+\\s*\\:.*");
const std::regex ConfigParser::PUNCTUATION_REGEX("\\[.*\\]");
const std::regex ConfigParser::KEYWORDS_REGEX("\\{.*\\}");

ConfigParser::ConfigParser(LanguageSpecParser* lang_sp) : lang_sp(lang_sp) {
  this->regex_table = new RegularExpressionTable();
}

ConfigParser::~ConfigParser() {
    delete regex_table;
    delete lang_sp;
}

std::string ConfigParser::getConfigFilePath() {
  return CONFIG_FILE_PATH;
}

void ConfigParser::readLanguageSpec(std::ifstream* input_file) {
  validateInputFile(input_file);
  readLanguage(input_file);
}

void ConfigParser::readLanguage(std::ifstream* input_file) {
  std::string current_line;
  int priority = -1;
  while(getline(*input_file, current_line)) {
    parseLine(current_line, priority);
    std::cout << " : " << current_line << std::endl;
    priority--;
  }
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
  lang_sp->parseRegularExpression(operand_2, regex_table);
}

void ConfigParser::parseRegularDefinition(std::string current_line, int priority) {
  std::string operand_1, operand_2;
  disassembleExpression(current_line, DEFINITION_OPERATOR, operand_1, operand_2);
  lang_sp->parseRegularExpression(operand_2, regex_table);

}

void ConfigParser::parsePunctuation(std::string current_line, int priority) {

}

void ConfigParser::parseKeywords(std::string current_line, int priority) {

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
