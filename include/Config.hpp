#ifndef Config_H
#define Config_H

#include <map>
#include <string>
#include <set>
#include <regex>

class Config {
public:
    ~Config();
    static Config* getInstance();
    void init(int, const char**);

    std::string getTokenOutputPath();
    std::string getInputProgramPath();
    std::string getLanguageSpecPath();
    std::string getTransitionTablePath();
    std::string getMinTransitionTablePath();
private:
    std::map<std::string, std::string> key_to_value;
    std::set<std::string> valid_keys;

    static Config* instance;

    const static std::string DEFAULT_SETTINGS_FILE;

    const static std::regex SETTING_REGEX;

    const static char SETTING_SEPARATOR;

    const static std::string INPUT_PROGRAM_PATH_KEY;
    const static std::string LANGUAGE_SPEC_PATH_KEY;
    const static std::string TOKEN_OUTPUT_PATH_KEY ;
    const static std::string TRANSITION_TABLE_PATH_KEY;
    const static std::string MIN_TRANSITION_TABLE_PATH_KEY;

    const static std::string DEFAULT_INPUT_PROGRAM_PATH;
    const static std::string DEFAULT_LANGUAGE_SPEC_PATH;
    const static std::string DEFAULT_TOKEN_OUTPUT_PATH;
    const static std::string DEFAULT_TRANSITION_TABLE_PATH;
    const static std::string DEFAULT_MIN_TRANSITION_TABLE_PATH;

    Config();
    void readFileProperties();
    void readCommandLineProperties(int, const char**);
    bool validKey(std::string);
};
#endif
