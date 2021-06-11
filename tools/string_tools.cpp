#include "string_tools.hpp"

bool tools::ends_with(const std::string &a, const std::string &end) {
#ifdef CMAKE_CXX_STANDARD
  if (CMAKE_CXX_STANDARD == 20){
    return a.ends_with(end);
  }
#endif
  if (end.size() > a.size()) {
    return false;
  }
  for (int i = 0; i < end.size(); ++i) {
    if (a[a.size() - 1 - i] != end[end.size() - 1 - i]) {
      return false;
    }
  }
  return true;
}