#include "..\..\include\lexical_analyzer/LexicalErrorReporter.hpp"
#include <cstdlib>

LexicalErrorReporter* LexicalErrorReporter::instance = NULL;

LexicalErrorReporter::LexicalErrorReporter() {
};

LexicalErrorReporter* LexicalErrorReporter::getInstance() {
        if(instance == NULL) {
                instance = new LexicalErrorReporter;
        }
        return instance;
}

void LexicalErrorReporter::setLine(int line) {
        this->current_line = line;
}

void LexicalErrorReporter::report(ReportMechanism report_mech, ErrorType error_type, std::vector<std::string> params) {
        std::string error_message = buildErrorMessage(error_type, params);
        std::cerr << error_message << std::endl;
        if(report_mech == ReportMechanism::REPORT_AND_EXIT) {
                std::cerr << "Program is exiting...\n \\o babai" << std::endl;
                exit(-1);
        }
}

std::string LexicalErrorReporter::buildErrorMessage(ErrorType error_type, std::vector<std::string> params) {
        std::stringstream ss;
        switch(error_type) {
        case ErrorType::INVALID_LINE_SPEC:
                ss << "Line " << current_line <<  " doesn't follow any valid specification.\nPlease refer to the documentation.";
                break;
        case ErrorType::MISSING_INPUT_FILE:
                ss << "Input file is missing. Make sure to creat a file with the name \'input.txt\'";
                break;
        case ErrorType::INVALID_PUNCTUATION:
                ss << "Invalid punctuation specified {" << params[0] << "}.\nMake sure its size is equal to one.";
                break;
        case ErrorType::INVALID_KEYWORD:
                ss << "Invalid keyword specified {" << params[0] << "}.\nMake sure its valid.";
                break;
        case ErrorType::IMBALANCED_BRACKETS:
                ss << "Brackets aren't balanced in the expression in line no " << current_line << ".\nPlease make sure to review the expression.";
                break;
        case ErrorType::INVALID_EXPRESSION:
                ss << "Invalid expression specified in line no " << current_line << ".\nPlease make sure to review the expression.";
                break;
        case ErrorType::MISSING_SYMBOL:
                ss << "Unknown regular expression used {" << params[0] << "} in line no " << current_line << ".\nPlease make sure it's defined.";
                break;
        case ErrorType::INVALID_REGEX_RANGE:
                ss << "Invalid Range {" << params[0] << "} in line no " << current_line << ".\nPlease only use a-z, A-Z or 0-9.";
                break;
        default:
                exit(-1);
        }
        return ss.str();
}
