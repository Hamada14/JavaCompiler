//
//  main.cpp
//  JavaCompiler
//
//  Created by Ahmed on 3/13/18.
//  Copyright © 2018 Abdellah. All rights reserved.
//
#include "ConfigParser.hpp"
#include <fstream>
#include <iostream>

void readLanguaeSpecs() {
    std::ifstream input_file;
    ConfigParser* config_parser = new ConfigParser(new LanguageSpecParser());
    std::string config_file_path = config_parser->getConfigFilePath();
    input_file.open(config_file_path);
    config_parser->readLanguageSpec(&input_file);
}

int main(int argc, const char * argv[]) {
    readLanguaeSpecs();
    return 0;
}
