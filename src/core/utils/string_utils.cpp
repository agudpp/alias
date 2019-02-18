#include <core/utils/string_utils.h>

#include <algorithm>
#include <fstream>
#include <sstream>


namespace core {

std::string
StringUtils::shortestCommonSuffix(const std::vector<std::string>& words, const std::string& prefix)
{
  std::vector<std::string> filtered = words;
  auto last = std::remove_if(filtered.begin(), filtered.end(), [&prefix](const std::string& w) {
    return w.find(prefix, 0) != 0;
  });
  filtered.erase(last, filtered.end());

  std::string suffix;
  if (filtered.empty()) {
    return prefix + suffix;
  }

  std::size_t min_len = filtered.back().size();
  for (const std::string& w : filtered) {
    min_len = std::min(min_len, w.size());
  }
  if (min_len == prefix.size()) {
    return prefix + suffix;
  }

  for (std::size_t i = prefix.size(); i < min_len; ++i) {
    char c = filtered.front()[i];
    for (const std::string& w : filtered) {
      if (w[i] != c) {
        return prefix + suffix;
      }
    }
    suffix += c;
  }

  return prefix + suffix;
}


std::vector<std::string>
StringUtils::splitStr(const std::string& str, char separator)
{
  std::vector<std::string> parts;
  std::istringstream f(str);
  std::string s;
  while (std::getline(f, s, separator)) {
      parts.push_back(s);
  }
  return parts;
}

} // namespace core

