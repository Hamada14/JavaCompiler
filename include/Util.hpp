#ifndef Util_hpp
#define Util_hpp

#include <string>
#include <vector>

namespace Util {

    bool isWhiteSpace(const char);
    bool isASCIIChar(const char);
    bool isASCIIDigit(const char);

    std::string trim(const std::string);
    std::vector<std::string> split(std::string, char = ' ');

    std::string int_to_string(int);
}
#endif
