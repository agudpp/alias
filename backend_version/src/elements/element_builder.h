#ifndef ELEMENT_BUILDER_H
#define ELEMENT_BUILDER_H

#include <memory>

#include <rapidjson/document.h>

#include <elements/element.h>

class ElementBuilder
{
  public:

    /**
     * @brief builds a element from a json document
     * @param doc the document containing the information of the element to be built
     * @return the pointer to the element if success | nullptr otherwise
     */
    static std::unique_ptr<Element>
    build(const rapidjson::Document& doc);

};

#endif // ELEMENT_BUILDER_H
