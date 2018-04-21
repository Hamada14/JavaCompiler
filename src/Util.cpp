#include "Util.hpp"

#include <iostream>
#include <sstream>

namespace Util {
  bool isWhiteSpace(const char c) {
          return c == ' ' || c == '\t';
  }

  bool isEmptyString(std::string str) {
    return str == EMPTY_STRING;
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
          for(size_t i = 0; i < str.length(); i++) {
                  if(str[i] == delimiter)
                          continue;
                  int j = i;
                  for(j = i; j < (int)str.length() && str[j] != delimiter; j++) ;
                  result.push_back(str.substr(i, j - i));
                  i = j - 1;
          }
          return result;
  }

  std::vector<std::string> splitQuoteSensitive(std::string str, char delimiter) {
      std::vector<std::string> result;
      bool inside_quotes = false;
      std::string partition = "";
      for(size_t i = 0; i < str.length(); i++) {
        if(!inside_quotes && str[i] == delimiter) {
          result.push_back(partition);
          partition = "";
        } else {
          partition += str[i];
          if(str[i] == SINGLE_QUOTE) {
            inside_quotes = !inside_quotes;
          }
        }
      }
      result.push_back(partition);
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
