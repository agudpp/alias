#include "element_builder.h"

#include <core/debug/Debug.h>

#include <elements/simple_text_element.h>


Element::Ptr
ElementBuilder::build(const std::string& type, std::istream& stream)
{
  Element::Ptr result;
  if (type == SimpleTextElement::NAME) {
    SimpleTextElement* element = new SimpleTextElement;
    if (!element->deserialize(stream)) {
      debugERROR("error deserializing the SimpleTextElement");
      delete element;
    } else {
      result.reset(element);
    }
  }
  return result;
}
