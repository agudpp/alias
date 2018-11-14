#ifndef JSON_UTILS_H
#define JSON_UTILS_H

#include <iostream>
#include <string>

#include <rapidjson/document.h>


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
};

}

#endif // JSON_UTILS_H
