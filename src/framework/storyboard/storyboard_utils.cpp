#pragma once

#include "../language/string.cpp"
#include <string>
#include <vector>

namespace StoryboardUtils {

static inline std::string parse_header(std::string s) {
  if (s.rfind("[", 0) == 0) {
    auto ss = s.replace(s.find("["), sizeof("[") - 1, "")
                  .replace(s.find("]"), sizeof("]") - 1, "");

    StringUtils::trim(ss);

    return ss;
  }

  return "";
}

static inline std::vector<std::string> parse_variables(std::string s) {
  auto ss = s;
  StringUtils::trim(ss);
  return StringUtils::split(ss, "=");
}

static inline std::vector<std::string> parse_commas(std::string s) {
  auto ss = s;
  StringUtils::trim(ss);
  return StringUtils::split(ss, ",");
}

} // namespace StoryboardUtils