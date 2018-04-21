#ifndef Util_hpp
#define Util_hpp

#include <string>
#include <vector>

namespace Util {
  const char WHITE_SPACE = ' ';
  const std::string EMPTY_STRING = "";
  const char SINGLE_QUOTE = '\'';

  bool isWhiteSpace(const char);
  bool isEmptyString(std::string);

  bool isASCIIChar(const char);
  bool isASCIIDigit(const char);

  std::string trim(const std::string);
  std::vector<std::string> split(std::string, char = ' ');
  std::vector<std::string> splitQuoteSensitive(std::string str, char delimiter);

  std::string int_to_string(int);
}
#endif // ifndef Util_hpp
