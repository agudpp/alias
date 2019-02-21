#include <ui_client/elements/types/element_ui_simple_text.h>

#include <cassert>

ElementUISimpleText::ElementUISimpleText()
{

}

QTableWidgetItem*
ElementUISimpleText::tableRepresentation(void) const
{
  assert(element().get() != nullptr);
  return new QTableWidgetItem(element()->getIndexingTest().c_str());
}
