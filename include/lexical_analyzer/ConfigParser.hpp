#ifndef ConfigParser_hpp
#define ConfigParser_hpp

#include "LanguageSpecParser.hpp"
#include "RegularExpressionTable.hpp"
#include "Util.hpp"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>

class ConfigParser {
  public:
    ConfigParser(LanguageSpecParser*);
    ~ConfigParser();

    std::string getConfigFilePath();
    NFA* readLanguageSpec(std::ifstream*);
  private:
    LanguageSpecParser* lang_sp;
    RegularExpressionTable* regex_table;
    RegularExpressionTable* definition_table;
    std::vector<NFA*> keywords, punctuations;

    static const char DEFINITION_OPERATOR;
    static const char EXPRESSION_OPERATOR;

    static const std::string CONFIG_FILE_PATH;

    static const std::regex REGULAR_EXPRESSION_REGEX;
    static const std::regex REGULAR_DEFINITION_REGEX;
    static const std::regex PUNCTUATION_REGEX;
    static const std::regex KEYWORDS_REGEX;

    void validateInputFile(std::ifstream*);
    NFA* readLanguage(std::ifstream*);

    bool isValidKeywords(std::string);
    bool isValidPunctuation(std::string);

    NFA* keywordToNFA(std::string, int priority);

    void parseLine(std::string, int);
    void parseRegularExpression(std::string, int);
    void parseRegularDefinition(std::string, int);
    void parsePunctuation(std::string, int);
    void parseKeywords(std::string, int);

    void disassembleExpression(std::string, char, std::string&, std::string&);

    NFA* getResultNFA();
};
#endif
