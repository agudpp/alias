#ifndef ELEMENT_BUILDER_H
#define ELEMENT_BUILDER_H

#include <string>
#include <istream>

#include <rapidjson/document.h>

#include <elements/element.h>

class ElementBuilder
{
  public:

    /**
     * @brief builds a element from a type and stream
     * @param type the element type
     * @param stream the stream
     * @return the pointer to the element if success | nullptr otherwise
     */
    static Element::Ptr
    build(const std::string& type, std::istream& stream);

    /**
     * @brief Builds an element from a json value
     * @param root_value the root value of the json containing the element
     * @param assign_new_id will assign a new id if missing, otherwise will use the current
     *                      (if not present and this flag = false => fails)
     * @return the element pointer on succes | false otherwise
     */
    static Element::Ptr
    build(const rapidjson::Value& root_value, bool assign_new_id = false);

};

#endif // ELEMENT_BUILDER_H
