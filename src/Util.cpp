#include "Util.hpp"

#include <iostream>
#include <sstream>

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
          for(start = 0; start < (int)str.length() && isWhiteSpace(str[start]); start++) ;
          for(end = (int)str.length() - 1; end >= 0 && isWhiteSpace(str[end]); end--) ;
          if(end <= start) return EMPTY_STRING;
          return str.substr(start, end - start + 1);
  }

  std::vector<std::string> split(std::string str, char delimiter) {
          std::vector<std::string> result;
          for(int i = 0; i < (int)str.length(); i++) {
                  if(str[i] == delimiter)
                          continue;
                  int j = i;
                  for(j = i; j < (int)str.length() && str[j] != delimiter; j++) ;
                  result.push_back(str.substr(i, j - i));
                  i = j - 1;
          }
          return result;
  }

  std::string int_to_string(int num)
  {
          std::stringstream ss;
          ss << num;
          std::string ret;
          ss >> ret;
          return ret;
  }
}
