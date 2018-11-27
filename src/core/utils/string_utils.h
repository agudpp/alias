#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <vector>
#include <string>

namespace core {

class StringUtils
{
  public:

    /**
     * @brief Method that will return the longest word with the common prefix
     * @param words the words
     * @param prefix the prefix to filter the interested words
     * @return the prefix + min_common_suffix for those words
     */
    static std::string
    shortestCommonSuffix(const std::vector<std::string>& words, const std::string& prefix);
};


} // namespace core

#endif // STRING_UTILS_H
