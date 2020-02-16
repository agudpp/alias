#include <toolbox/utils/string_utils.h>

#include <algorithm>
#include <fstream>
#include <sstream>

#include <toolbox/utils/random.h>


namespace toolbox {

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

std::vector<std::string>
StringUtils::splitChunks(const std::string& str, std::size_t the_chunk_size)
{
  std::vector<std::string> result;
  if (the_chunk_size == 0 || str.empty()) {
    return result;
  }

  const std::size_t total_size = str.size();
  std::size_t curr_pos = 0;
  while (curr_pos < total_size) {
    const std::size_t remaining = total_size - curr_pos;
    const std::size_t chunk_size = std::min(remaining, the_chunk_size);
    result.push_back(str.substr(curr_pos, chunk_size));
    curr_pos += chunk_size;
  }
  return result;
}

std::string
StringUtils::generateRandom(std::size_t count)
{
  std::string result;
  result.reserve(count);
  for (std::size_t i = 0; i < count; i++) {
    result.push_back(Random::getRandomChar());
  }
  return result;
}

std::string
StringUtils::duplicateTillNth(const std::string& str, std::size_t num_chars)
{
  if (num_chars == 0) {
    return "";
  }
  std::string result = str;
  if (str.empty() || result.size() == num_chars) {
    return result;
  }
  while (result.size() < num_chars) {
    result += str;
  }

  result.resize(num_chars);

  return result;
}


} // namespace toolbox

