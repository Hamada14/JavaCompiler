#ifndef Util_hpp
#define Util_hpp

#include <string>

namespace Util {

  bool isWhiteSpace(const char);
  bool isASCIIChar(const char);
  bool isASCIIDigit(const char);

  std::string trim(const std::string);
}
#endif
