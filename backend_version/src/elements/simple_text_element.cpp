#include "simple_text_element.h"

#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/document.h>

#include <core/debug/Debug.h>
#include <core/utils/json_utils.h>


const std::string SimpleTextElement::NAME = "SimpleTextElement";


bool
SimpleTextElement::loadFromJsonValue(rapidjson::Value& json_value)
{
  if (!Element::loadFromJsonValue(json_value)) {
    return false;
  }
  if (!json_value.IsObject() || !json_value.HasMember("text")) {
    debugERROR("Invalid json object, expected text field");
    return false;
  }
  // parse the data and set the elements
  const rapidjson::Value& txtVal = json_value["text"];
  setText(txtVal.GetString());

  return true;
}

rapidjson::Value
SimpleTextElement::toJsonValue(rapidjson::Document& d) const
{
  rapidjson::Value result = Element::toJsonValue(d);
  auto& al = d.GetAllocator();
  result.AddMember("text", text(), al);
  return result;
}


SimpleTextElement::SimpleTextElement(const core::UID& id, const std::string& text) :
  Element(id)
, text_(text)
{
}

std::string
SimpleTextElement::getIndexingTest(void) const
{
  return text_;
}

std::string
SimpleTextElement::elementType(void) const
{
  return NAME;
}

bool
SimpleTextElement::serialize(std::ostream& stream) const
{
  rapidjson::Document d;
  d.SetObject() = toJsonValue(d);
  core::JsonUtils::toStream(d, stream);
  return true;
}


bool
SimpleTextElement::deserialize(std::istream& stream)
{
  rapidjson::Document doc = core::JsonUtils::fromStream(stream);
  return loadFromJsonValue(doc);
}

