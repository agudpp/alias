#include <ui_client/elements/element_executor.h>

#include <string>

#include <QClipboard>
#include <QApplication>

#include <core/debug/Debug.h>

#include <elements/simple_text_element.h>

/**
 * @brief executeSimpleText
 * @param data
 * @return
 */
static bool
executeSimpleText(const ElementExecutor::Data& data)
{
  const SimpleTextElement* elem = static_cast<const SimpleTextElement*>(data.element.get());
  QClipboard* clipboard = QApplication::clipboard();
  clipboard->setText(elem->text().c_str());
  return true;
}


bool
ElementExecutor::execute(const Data& data)
{
  ASSERT_PTR(data.element.get());
  const std::string elem_type = data.element->elementType();

  if (elem_type == "SimpleTextElement") {
    return executeSimpleText(data);
  }

  LOG_ERROR("Unknown element type, cannot be executed: %s", elem_type.c_str());
  return false;
}
