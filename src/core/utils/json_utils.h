#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <iostream>
#include <string>
#include <vector>

#include <rapidjson/document.h>

#include <core/debug/Debug.h>

namespace core {

class JsonUtils
{
  public:

    /**
     * @brief Reads a document from a stream
     * @param stream the stream
     * @return the document
     */
    static rapidjson::Document
    fromStream(std::istream& stream);

    /**
     * @brief Converts a document into a string
     * @param d the document
     * @return string associated
     */
    static std::string
    toString(rapidjson::Document& d);

    /**
     * @brief Serialize the document into the stream
     * @param d the document
     * @param stream the stream
     */
    static void
    toStream(rapidjson::Document& d, std::ostream& stream);

    /**
     * @brief Get a string from the given element d and specific field name
     * @return the associated string
     */
    static inline std::string
    getString(const rapidjson::Value& d, const char* field_name);

    /**
     * @brief Parse a string arrary from the given document
     * @param d
     * @param fieldName
     * @param result
     */
    static inline void
    getStrArray(const rapidjson::Document& d,
                const char* fieldName,
                std::vector<std::string>& result);


};







//////////////////////////////////////////////////////////////////////////////////////////
// inline
inline std::string
JsonUtils::getString(const rapidjson::Value& d, const char* field_name)
{
    const rapidjson::Value& queryVal = d[field_name];
    ASSERT(queryVal.IsString());
    return queryVal.GetString();
}

/**
 * @brief Parse a string arrary from the given document
 * @param d
 * @param fieldName
 * @param result
 */
inline void
JsonUtils::getStrArray(const rapidjson::Document& d,
            const char* fieldName,
            std::vector<std::string>& result)
{
    const rapidjson::Value& val= d[fieldName];
    ASSERT(val.IsArray());
    result.clear();
    result.reserve(val.Size());
    for (unsigned int i = 0; i < val.Size(); ++i) {
        ASSERT(val[i].IsString());
        result.push_back(val[i].GetString());
    }
}


}

#endif // JSON_UTILS_H
