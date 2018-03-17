#include "Util.hpp"

namespace Util {
  const std::string EMPTY_STRING = "";

  bool isWhiteSpace(const char c) {
    return c == ' ' || c == '\t';
  }

  bool isASCIIChar(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
  }

  bool isASCIIDigit(const char c) {
    return c >= '0' && c <= '9';
  }

  std::string trim(const std::string str) {
    int start, end;
    for(start = 0; start < str.length() && isWhiteSpace(str[start]); start++);
    for(end = str.length() - 1; end >= 0 && isWhiteSpace(str[end]); end--);
    if(end <= start) return EMPTY_STRING;
    return str.substr(start, end - start + 1);
  }
}
