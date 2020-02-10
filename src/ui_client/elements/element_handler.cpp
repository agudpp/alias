#include <ui_client/elements/element_handler.h>

#include <QDebug>

#include <core/debug/Debug.h>

#include <ui_client/elements/element_executor.h>
#include <ui_client/elements/types/element_ui_builder.h>

#include "ui_element_handler.h"





ElementHandler::ElementHandler(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ElementHandler)
{
  ui->setupUi(this);
  ui->verticalLayout->addWidget(table_.getRepresentation());
}

ElementHandler::~ElementHandler()
{
  clear();
  delete ui;
}

void
ElementHandler::setElements(const std::vector<Element::ConstPtr>& elements)
{
  clear();
  for (Element::ConstPtr element : elements) {
    ElementUIBase::Ptr ui_element = ElementUIBuilder::build(element);
    ASSERT_PTR(ui_element.get());
    elements_.push_back(ui_element);
    table_.addElement(ui_element.get());
  }
}

void
ElementHandler::clear(void)
{
  table_.clear();
  elements_.clear();
}

bool
ElementHandler::selectNext(void)
{
  table_.selectNext();
  return true;
}

bool
ElementHandler::selectPrev(void)
{
  table_.selectPrevious();
  return true;
}

void
ElementHandler::unselectCurrent(void)
{
  table_.unselectCurrent();
}

bool
ElementHandler::hasSelected(void) const
{
  return table_.hasSelected();
}

Element::ConstPtr
ElementHandler::selected(void)
{
  const ElementUIBase* ui_element = table_.selected();
  ASSERT_PTR(ui_element);
  return ui_element->element();
}

