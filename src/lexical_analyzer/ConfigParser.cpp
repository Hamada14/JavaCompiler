#include "lexical_analyzer/ConfigParser.hpp"

#include "lexical_analyzer/LexicalErrorReporter.hpp"

const int ConfigParser::MAX_PRIORITY = 0;
const std::string ConfigParser::CONFIG_FILE_PATH = "input";

const char ConfigParser::DEFINITION_OPERATOR = ':';
const char ConfigParser::EXPRESSION_OPERATOR = '=';

const std::regex ConfigParser::REGULAR_EXPRESSION_REGEX("[a-zA-Z][a-zA-Z0-9\\-_]+\\s*\\=.+");
const std::regex ConfigParser::REGULAR_DEFINITION_REGEX("[a-zA-Z][a-zA-Z0-9\\-_]+\\s*\\:.+");
const std::regex ConfigParser::PUNCTUATION_REGEX("\\[.*\\]");
const std::regex ConfigParser::KEYWORDS_REGEX("\\{.*\\}");

ConfigParser::ConfigParser(LanguageSpecParser* lang_sp) : lang_sp(lang_sp) {
        this->regex_table = new RegularExpressionTable();
        this->definition_table = new RegularExpressionTable();
}

ConfigParser::~ConfigParser() {
        for(NFA* keyword : keywords) {
                delete keyword;
        }
        for(NFA* punctuation : punctuations) {
                delete punctuation;
        }
        delete regex_table;
        delete lang_sp;
        delete definition_table;
}

std::string ConfigParser::getConfigFilePath() {
        return CONFIG_FILE_PATH;
}

NFA* ConfigParser::readLanguageSpec(std::ifstream* input_file) {
        validateInputFile(input_file);
        return readLanguage(input_file);
}

NFA* ConfigParser::readLanguage(std::ifstream* input_file) {
        std::string current_line;
        int priority = MAX_PRIORITY - 1;
        while(getline(*input_file, current_line)) {
                LexicalErrorReporter::getInstance()->setLine(-priority);
                parseLine(Util::trim(current_line), priority);
                priority--;
        }
        input_file->close();
        return getResultNFA();
}

NFA* ConfigParser::getResultNFA() {
        std::vector<NFA*> result_items = definition_table->getValues();
        result_items.insert(std::begin(result_items), std::begin(punctuations), std::end(punctuations));
        result_items.insert(std::begin(result_items), std::begin(keywords), std::end(keywords));
        return NFA::combine(result_items);
}

void ConfigParser::parseLine(std::string current_line, int priority) {
        if(regex_match(current_line, REGULAR_EXPRESSION_REGEX)) {
                parseRegularExpression(current_line);
        } else if(regex_match(current_line, REGULAR_DEFINITION_REGEX)) {
                parseRegularDefinition(current_line, priority);
        } else if(regex_match(current_line, PUNCTUATION_REGEX)) {
                parsePunctuation(current_line);
        } else if(regex_match(current_line, KEYWORDS_REGEX)) {
                parseKeywords(current_line);
        } else {
                LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::INVALID_LINE_SPEC, {});
        }
}

void ConfigParser::parseRegularExpression(std::string current_line) {
        std::string operand_1, operand_2;
        disassembleExpression(current_line, EXPRESSION_OPERATOR, operand_1, operand_2);
        NFA* result = lang_sp->parseRegularExpression(operand_2, regex_table);
        regex_table->addExpressionNFA(operand_1, result);
}

void ConfigParser::parseRegularDefinition(std::string current_line, int priority) {
        std::string operand_1, operand_2;
        disassembleExpression(current_line, DEFINITION_OPERATOR, operand_1, operand_2);
        NFA* result = lang_sp->parseRegularExpression(operand_2, regex_table);
        result->set_acceptance(operand_1, priority);
        definition_table->addExpressionNFA(operand_1, result);
}

void ConfigParser::parsePunctuation(std::string current_line) {
        std::vector<std::string> splitted_punctuations = Util::split(current_line.substr(1, current_line.length() - 2));
        for(std::string punc : splitted_punctuations) {
                if(!isValidPunctuation(punc)) {
                        LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                        reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::INVALID_PUNCTUATION, {punc});
                }
                punctuations.push_back(punctuationToNFA(punc.substr(punc.length() - 1)[0], MAX_PRIORITY));
        }
}

void ConfigParser::parseKeywords(std::string current_line) {
        std::vector<std::string> splitted_keywords = Util::split(current_line.substr(1, current_line.length() - 2));
        for(std::string keyword : splitted_keywords) {
                if(!isValidKeywords(keyword)) {
                        LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                        reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::INVALID_KEYWORD, {keyword});
                }
                keywords.push_back(keywordToNFA(keyword, MAX_PRIORITY));
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
        NFA* result = new NFA(keyword);
        result->set_acceptance(keyword, priority);
        return result;
}

NFA* ConfigParser::punctuationToNFA(char ch, int priority) {
        NFA* result = new NFA(ch);
        result->set_acceptance(string(1, ch), priority);
        return result;
}

void ConfigParser::disassembleExpression(std::string expression, char operator_, std::string& operand_1, std::string& operand_2) {
        int operator_index = (int)expression.find(operator_);
        operand_1 = Util::trim(expression.substr(0, operator_index));
        operand_2 = Util::trim(expression.substr(operator_index + 1));
}

void ConfigParser::validateInputFile(std::ifstream* input_file) {
        if(!*input_file) {
                LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::MISSING_INPUT_FILE, {});
        }
}
