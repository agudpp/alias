#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <vector>
#include <string>

namespace toolbox {

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

    /**
     * @brief Split the string into chunks of the given size
     * @param str the string to split
     * @param chunk_size the chunk size
     * @return the string chunks parts {[0, chunk_size), [chunk_size, 2 * chunk_size),...}
     */
    static std::vector<std::string>
    splitChunks(const std::string& str, std::size_t chunk_size);

    /**
     * @brief Generate a random string of size "count"
     * @param count the number of bytes to be randomly generated
     * @return the string of size == count filled with random values
     */
    static std::string
    generateRandom(std::size_t count);

    /**
     * @brief Duplicate the same string till filling num_chars, and then cut if required
     * @param str the string to be duplicated
     * @param num_chars the number of characters we want to end up having
     * @return the string with num_chars
     */
    static std::string
    duplicateTillNth(const std::string& str, std::size_t num_chars);

};


} // namespace toolbox

#endif // STRING_UTILS_H
