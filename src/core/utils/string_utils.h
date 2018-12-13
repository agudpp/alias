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

    /**
     * @brief Split a string into parts using the given separator
     * @param str the string to split
     * @param separator the separator
     * @return the parts or empty if none
     */
    static std::vector<std::string>
    splitStr(const std::string& str, char separator);
};


} // namespace core

#endif // STRING_UTILS_H
