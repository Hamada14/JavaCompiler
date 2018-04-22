#ifndef RULETOKEN_HPP
#define RULETOKEN_HPP

#include <string>
#include <regex>
#include <set>

enum class RuleTokenType { TERMINAL, NON_TERMINAL, LAMBDA_TERMINAL};

class RuleToken {
public:
    const static std::string LAMBDA_VALUE;
    const static char BACK_SLASH;

    RuleToken(std::string value);
    ~RuleToken();

    RuleTokenType getType();
    std::string getValue();

    bool isInvalid();

private:
    const static std::regex TERMINAL_TOKEN_REGEX;
    const static std::regex NON_TERMINAL_TOKEN_REGEX;

    const static std::set<char> RESERVED_SYMBOLS;

    std::string value;
    RuleTokenType type;

    bool is_invalid;

    bool checkEscapeCharacters();
    bool isReservedSymbol(char symbol);
};
#endif // ifndef RULETOKEN_HPP
