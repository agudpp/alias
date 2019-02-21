#include <ui_client/elements/types/element_ui_builder.h>

#include <core/debug/Debug.h>

#include <elements/simple_text_element.h>

#include <ui_client/elements/types/element_ui_simple_text.h>


ElementUIBase::Ptr
ElementUIBuilder::build(Element::ConstPtr& element)
{
  ASSERT_PTR(element.get());

  ElementUIBase::Ptr result;
  const std::string elem_type = element->elementType();

  if (elem_type == SimpleTextElement::NAME) {
    result.reset(new ElementUISimpleText);
  } else {
    LOG_ERROR("We do not have the element with type: %s", elem_type.c_str());
  }

  if (result.get() != nullptr) {
    result->setElement(element);
  }

  return result;
}
