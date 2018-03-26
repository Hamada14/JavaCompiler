#include "Config.hpp"

#include "Util.hpp"

#include <fstream>
#include <iostream>

Config* Config::instance = NULL;

const std::string Config::DEFAULT_SETTINGS_FILE = "config";

const std::regex Config::SETTING_REGEX("[a-zA-Z][a-zA-Z0-9\\-_]+\\s*\\=.+");

const char Config::SETTING_SEPARATOR = '=';

const std::string Config::INPUT_PROGRAM_PATH_KEY = "input";
const std::string Config::LANGUAGE_SPEC_PATH_KEY = "langSpec";
const std::string Config::TOKEN_OUTPUT_PATH_KEY = "tokenOutput";
const std::string Config::TRANSITION_TABLE_PATH_KEY = "transitionTable";
const std::string Config::MIN_TRANSITION_TABLE_PATH_KEY = "minTransitionTable";

const std::string Config::DEFAULT_INPUT_PROGRAM_PATH = "tokens.txt";
const std::string Config::DEFAULT_LANGUAGE_SPEC_PATH = "input.txt";
const std::string Config::DEFAULT_TOKEN_OUTPUT_PATH = "token_out.txt";
const std::string Config::DEFAULT_TRANSITION_TABLE_PATH = "transition_table.txt";
const std::string Config::DEFAULT_MIN_TRANSITION_TABLE_PATH = "minimal_transition_table.txt";

Config::Config() {
        this->key_to_value = {
                {INPUT_PROGRAM_PATH_KEY, DEFAULT_INPUT_PROGRAM_PATH}, {LANGUAGE_SPEC_PATH_KEY, DEFAULT_LANGUAGE_SPEC_PATH},
                {TOKEN_OUTPUT_PATH_KEY, DEFAULT_TOKEN_OUTPUT_PATH}, {TRANSITION_TABLE_PATH_KEY, DEFAULT_TRANSITION_TABLE_PATH},
                {MIN_TRANSITION_TABLE_PATH_KEY, DEFAULT_MIN_TRANSITION_TABLE_PATH}
        };

        this->valid_keys = {
                INPUT_PROGRAM_PATH_KEY, LANGUAGE_SPEC_PATH_KEY, TOKEN_OUTPUT_PATH_KEY,
                TRANSITION_TABLE_PATH_KEY, MIN_TRANSITION_TABLE_PATH_KEY
        };
}


Config::~Config(){
}

Config* Config::getInstance() {
        if(instance != NULL) {
                return instance;
        }
        return instance = new Config();
}

void Config::init(int argc, const char** argv) {
        readFileProperties();
        readCommandLineProperties(argc, argv);
}

void Config::readFileProperties() {
        std::ifstream settings_file;
        settings_file.open(DEFAULT_SETTINGS_FILE);
        std::string current_line;
        int i = 0;
        while(getline(settings_file, current_line)) {
                i++;
                if(!regex_match(current_line, SETTING_REGEX)) {
                        std::cerr << "Invalid settings file line at {" << i << "}th line... \nRemaining parameters are ignored." << std::endl;
                        exit(-1);
                }
                int sep_pos = current_line.find(SETTING_SEPARATOR);
                std::string key = Util::trim(current_line.substr(0, sep_pos));
                std::string val = Util::trim(current_line.substr(sep_pos + 1));
                if(!validKey(key)) {
                        std::cerr << "Invalid Parameter key {" << current_line << "}" << std::endl;
                        exit(-1);
                }
                key_to_value[key] = val;
        }
        settings_file.close();
}

void Config::readCommandLineProperties(int argc, const char** argv) {
        if(argc % 2 != 1) {
                std::cerr << "Invalid command line parameters...Parameters'll be ignored" << std::endl;
                exit(-1);
        }
        for(int i = 1; i < argc; i += 2) {
                std::string str(argv[i]);
                if(str.length() > 1 && str[0] == '-' && validKey(str.substr(0))) {
                        key_to_value[str.substr(0)] = std::string(argv[i + 1]);
                } else {
                        std::cerr << "Invalid Parameter key {" << str << "}" << std::endl;
                        exit(-1);
                }
        }
}

bool Config::validKey(std::string key) {
        return valid_keys.find(key) != valid_keys.end();
}

std::string Config::getTokenOutputPath() {
        return key_to_value[TOKEN_OUTPUT_PATH_KEY];
}

std::string Config::getInputProgramPath() {
        return key_to_value[INPUT_PROGRAM_PATH_KEY];
}

std::string Config::getLanguageSpecPath() {
        return key_to_value[LANGUAGE_SPEC_PATH_KEY];
}

std::string Config::getTransitionTablePath() {
        return key_to_value[TRANSITION_TABLE_PATH_KEY];
}

std::string Config::getMinTransitionTablePath() {
        return key_to_value[MIN_TRANSITION_TABLE_PATH_KEY];
}
