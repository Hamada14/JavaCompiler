#ifndef RULETOKEN_HPP
#define RULETOKEN_HPP

#include <string>
#include <regex>

enum class RuleTokenType { TERMINAL, NON_TERMINAL, LAMBDA_TERMINAL};

class RuleToken {
public:
    const static std::string LAMBDA_VALUE;

    RuleToken(std::string value);
    ~RuleToken();

    RuleTokenType getType();
    std::string getValue();

    bool isInvalid();

private:
    const static std::regex TERMINAL_TOKEN_REGEX;
    const static std::regex NON_TERMINAL_TOKEN_REGEX;

    std::string value;
    RuleTokenType type;

    bool is_invalid;
};
#endif // ifndef RULETOKEN_HPP
