#ifndef Lexical_Error_Reporter_hpp
#define Lexical_Error_Reporter_hpp

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

enum class ReportMechanism {REPORT, REPORT_AND_EXIT};
enum class ErrorType { INVALID_LINE_SPEC, MISSING_INPUT_FILE, INVALID_KEYWORD,
                       INVALID_PUNCTUATION, IMBALANCED_BRACKETS,INVALID_EXPRESSION,
                       MISSING_SYMBOL,INVALID_REGEX_RANGE
                   };

class LexicalErrorReporter {
public:
    static LexicalErrorReporter* getInstance();
    void setLine(int);
    void report(ReportMechanism, ErrorType, std::vector<std::string>);

private:
    static LexicalErrorReporter* instance;

    LexicalErrorReporter();
    std::string buildErrorMessage(ErrorType, std::vector<std::string>);

    int current_line;
};
#endif
