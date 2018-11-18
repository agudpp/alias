#ifndef ELEMENT_BUILDER_H
#define ELEMENT_BUILDER_H

#include <string>
#include <istream>

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

};

#endif // ELEMENT_BUILDER_H
