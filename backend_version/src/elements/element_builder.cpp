#include "element_builder.h"

#include <core/debug/Debug.h>

std::unique_ptr<Element>
ElementBuilder::build(const rapidjson::Document& doc)
{
  ASSERT(false && "TODO: implement this");
  return std::unique_ptr<Element>(nullptr);
}
