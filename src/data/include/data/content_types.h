#ifndef DATA_CONTENT_TYPES_H_
#define DATA_CONTENT_TYPES_H_

#include <map>
#include <vector>
#include <string>


namespace data {

enum class ContentType {
  // undefined type...
  UNDEFINED = -1,
  // simple text type (for example copy to clipboard)
  TEXT = 1,
  // simple command
  COMMAND = 2,

};


static const std::map<ContentType, std::string> CONTENT_TYPE_MAP {
  { ContentType::TEXT, "TEXT" },
  { ContentType::COMMAND, "COMMAND" },
};

/**
 * @brief Converts the ContentType to a string
 * @param content_type The content type to convert into string
 * @return the string version of the ContentType
 */
static inline std::string
toString(ContentType content_type)
{
  auto itr = CONTENT_TYPE_MAP.find(content_type);
  return itr == CONTENT_TYPE_MAP.end() ? "UNDEFINED" : itr->second;
}

static inline ContentType
fromString(const std::string& content_type_str)
{
  for (auto& itr : CONTENT_TYPE_MAP) {
    if (itr.second == content_type_str) {
      return itr.first;
    }
  }
  return ContentType::UNDEFINED;
}

/**
 * @brief Return all the ContentType in string mode
 * @return all the ContentType in string mode
 */
static inline std::vector<std::string>
contentTypeStrings()
{
  std::vector<std::string> result;
  for (auto& itr : CONTENT_TYPE_MAP) {
    result.push_back(itr.second);
  }
  return result;
}


}

#endif // DATA_CONTENT_TYPES_H_
