#include "element_builder.h"

#include <core/debug/Debug.h>
#include <core/utils/json_utils.h>

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

Element::Ptr
ElementBuilder::build(const rapidjson::Value& root_value, bool assign_new_id)
{
  Element::Ptr result;
  if (!root_value.HasMember("type")) {
    debugERROR("Cannot get the type on the json value");
    return result;
  }
  if (!assign_new_id && !root_value.HasMember("id")) {
    debugERROR("We need the id but is not present");
    return result;
  }

  assert(false && "TODO: get the type and build depending on it, we may want to add the tags?");

  return result;
}

