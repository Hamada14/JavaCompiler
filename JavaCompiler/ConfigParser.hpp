#ifndef ConfigParser_hpp
#define ConfigParser_hpp

#include <fstream>
#include <iostream>
#include <string>
#include <regex>
#include "Util.hpp"
#include "LanguageSpecParser.hpp"
#include "RegularExpressionTable.hpp"

class ConfigParser {
  public:
    ConfigParser(LanguageSpecParser*);
    ~ConfigParser();
    
    std::string getConfigFilePath();
    void readLanguageSpec(std::ifstream*);
  private:
    LanguageSpecParser* lang_sp;
    RegularExpressionTable* regex_table;

    static const char DEFINITION_OPERATOR;
    static const char EXPRESSION_OPERATOR;

    static const std::string CONFIG_FILE_PATH;

    static const std::regex REGULAR_EXPRESSION_REGEX;
    static const std::regex REGULAR_DEFINITION_REGEX;
    static const std::regex PUNCTUATION_REGEX;
    static const std::regex KEYWORDS_REGEX;

    void validateInputFile(std::ifstream*);
    void readLanguage(std::ifstream*);

    void parseLine(std::string, int);
    void parseRegularExpression(std::string, int);
    void parseRegularDefinition(std::string, int);
    void parsePunctuation(std::string, int);
    void parseKeywords(std::string, int);

    void disassembleExpression(std::string, char, std::string&, std::string&);
};
#endif
