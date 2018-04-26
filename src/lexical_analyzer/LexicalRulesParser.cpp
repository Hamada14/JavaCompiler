#include "..\..\include\lexical_analyzer/LexicalRulesParser.hpp"

#include "..\..\include\lexical_analyzer/LexicalErrorReporter.hpp"

#include <ctime>
#include <ratio>
#include <chrono>

const int LexicalRulesParser::MAX_PRIORITY = 0;

const char LexicalRulesParser::DEFINITION_OPERATOR = ':';
const char LexicalRulesParser::EXPRESSION_OPERATOR = '=';

const std::regex LexicalRulesParser::REGULAR_EXPRESSION_REGEX("[a-zA-Z][a-zA-Z0-9\\-_]+\\s*\\=.+");
const std::regex LexicalRulesParser::REGULAR_DEFINITION_REGEX("[a-zA-Z][a-zA-Z0-9\\-_]+\\s*\\:.+");
const std::regex LexicalRulesParser::PUNCTUATION_REGEX("\\[.*\\]");
const std::regex LexicalRulesParser::KEYWORDS_REGEX("\\{.*\\}");

LexicalRulesParser::LexicalRulesParser(LanguageSpecParser* lang_sp) : lang_sp(lang_sp) {
        this->regex_table = new RegularExpressionTable();
        this->definition_table = new RegularExpressionTable();
}

LexicalRulesParser::~LexicalRulesParser() {
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

Tokenizer* LexicalRulesParser::getLanguageTokenizer(std::ifstream* input_file) {
        validateInputFile(input_file);

        std::chrono::high_resolution_clock::time_point t1, t2;
        std::chrono::duration<double> time_span;

        std::cout << "Building NFA..." << std::endl;
        t1 = std::chrono::high_resolution_clock::now();

        NFA* nfa = readLanguage(input_file);

        t2 = std::chrono::high_resolution_clock::now();
        time_span = std::chrono::duration_cast<std::chrono::duration<double> >(t2 - t1);
        printf("Built NFA successfully in %.3f sec\n", time_span.count());
        printf("Final NFA Size is %d\n", nfa->get_graph()->get_size());

        return new Tokenizer(nfa);
}

NFA* LexicalRulesParser::readLanguage(std::ifstream* input_file) {
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

NFA* LexicalRulesParser::getResultNFA() {
        std::vector<NFA*> result_items = definition_table->getValues();
        result_items.insert(std::begin(result_items), std::begin(punctuations), std::end(punctuations));
        result_items.insert(std::begin(result_items), std::begin(keywords), std::end(keywords));
        return NFA::combine(result_items);
}

void LexicalRulesParser::parseLine(std::string current_line, int priority) {
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

void LexicalRulesParser::parseRegularExpression(std::string current_line) {
        std::string operand_1, operand_2;
        disassembleExpression(current_line, EXPRESSION_OPERATOR, operand_1, operand_2);
        NFA* result = lang_sp->parseRegularExpression(operand_2, regex_table);
        regex_table->addExpressionNFA(operand_1, result);
}

void LexicalRulesParser::parseRegularDefinition(std::string current_line, int priority) {
        std::string operand_1, operand_2;
        disassembleExpression(current_line, DEFINITION_OPERATOR, operand_1, operand_2);
        NFA* result = lang_sp->parseRegularExpression(operand_2, regex_table);
        result->set_acceptance(operand_1, priority);
        definition_table->addExpressionNFA(operand_1, result);
}

void LexicalRulesParser::parsePunctuation(std::string current_line) {
        std::vector<std::string> splitted_punctuations = Util::split(current_line.substr(1, current_line.length() - 2));
        for(std::string punc : splitted_punctuations) {
                if(!isValidPunctuation(punc)) {
                        LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                        reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::INVALID_PUNCTUATION, {punc});
                }
                punctuations.push_back(punctuationToNFA(punc.substr(punc.length() - 1)[0], MAX_PRIORITY));
        }
}

void LexicalRulesParser::parseKeywords(std::string current_line) {
        std::vector<std::string> splitted_keywords = Util::split(current_line.substr(1, current_line.length() - 2));
        for(std::string keyword : splitted_keywords) {
                if(!isValidKeywords(keyword)) {
                        LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                        reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::INVALID_KEYWORD, {keyword});
                }
                keywords.push_back(keywordToNFA(keyword, MAX_PRIORITY));
        }
}

bool LexicalRulesParser::isValidKeywords(std::string keyword) {
        for(int i = 0; i < (int)keyword.length(); i++) {
                if(lang_sp->isReservedSymbol(keyword[i]) || (keyword[i] == '\\' && i != (int)keyword.length() - 1
                                                             && !lang_sp->isReservedSymbol(keyword[i + 1])))
                        return false;
                if(keyword[i] == '\\') i++;
        }
        return true;
}

bool LexicalRulesParser::isValidPunctuation(std::string punc) {
        if((punc.length() == 1 && !lang_sp->isReservedSymbol(punc[0])) ||
           (punc.length() == 2 && punc[0] == '\\' && lang_sp->isReservedSymbol(punc[1]))) {
                return true;
        }
        return false;
}

NFA* LexicalRulesParser::keywordToNFA(std::string keyword, int priority) {
        NFA* result = new NFA(keyword);
        result->set_acceptance(keyword, priority);
        return result;
}

NFA* LexicalRulesParser::punctuationToNFA(char ch, int priority) {
        NFA* result = new NFA(ch);
        result->set_acceptance(string(1, ch), priority);
        return result;
}

void LexicalRulesParser::disassembleExpression(std::string expression, char operator_, std::string& operand_1, std::string& operand_2) {
        int operator_index = (int)expression.find(operator_);
        operand_1 = Util::trim(expression.substr(0, operator_index));
        operand_2 = Util::trim(expression.substr(operator_index + 1));
}

void LexicalRulesParser::validateInputFile(std::ifstream* input_file) {
        if(!*input_file) {
                LexicalErrorReporter* reporter = LexicalErrorReporter::getInstance();
                reporter->report(ReportMechanism::REPORT_AND_EXIT, ErrorType::MISSING_INPUT_FILE, {});
        }
}
