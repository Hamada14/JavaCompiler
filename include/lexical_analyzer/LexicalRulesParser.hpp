#ifndef LexicalRulesParser_hpp
#define LexicalRulesParser_hpp

#include <fstream>
#include <iostream>
#include <regex>
#include <string>

#include "LanguageSpecParser.hpp"
#include "..\..\include\lexical_analyzer/Tokenizer.hpp"
#include "RegularExpressionTable.hpp"
#include "..\..\include\Util.hpp"

class LexicalRulesParser {
public:
    LexicalRulesParser(LanguageSpecParser*);
    ~LexicalRulesParser();

    Tokenizer* getLanguageTokenizer(std::ifstream*);

private:
    static const int MAX_PRIORITY;
    static const char DEFINITION_OPERATOR;
    static const char EXPRESSION_OPERATOR;

    static const std::string CONFIG_FILE_PATH;

    static const std::regex REGULAR_EXPRESSION_REGEX;
    static const std::regex REGULAR_DEFINITION_REGEX;
    static const std::regex PUNCTUATION_REGEX;
    static const std::regex KEYWORDS_REGEX;

    LanguageSpecParser* lang_sp;
    RegularExpressionTable* regex_table;
    RegularExpressionTable* definition_table;
    std::vector<NFA*> keywords, punctuations;

    void validateInputFile(std::ifstream*);
    NFA* readLanguage(std::ifstream*);

    bool isValidKeywords(std::string);
    bool isValidPunctuation(std::string);

    NFA* keywordToNFA(std::string, int priority);
    NFA* punctuationToNFA(char, int);

    void parseLine(std::string, int);
    void parseRegularExpression(std::string);
    void parseRegularDefinition(std::string, int);
    void parsePunctuation(std::string);
    void parseKeywords(std::string);

    void disassembleExpression(std::string, char, std::string&, std::string&);

    NFA* getResultNFA();

    Tokenizer* NFAToTokenizer(NFA*);
};
#endif
