#include "element_handler.h"

#include <QDebug>

#include <core/debug/Debug.h>

#include <ui_client/elements/element_executor.h>

#include "ui_element_handler.h"



ElementWidget*
ElementHandler::itemFromElement(Element::ConstPtr& element)
{
  ElementWidget* result = nullptr;
  if (elem_queue_.empty()) {
    result = new ElementWidget();
  } else {
    result = elem_queue_.back();
    elem_queue_.pop_back();
  }
  result->setElement(element);
  return result;
}

void
ElementHandler::freeItem(ElementWidget* item)
{
  ASSERT_PTR(item);
  qDebug() << "freeing item";
  elem_queue_.push_back(item);
}

ElementWidget*
ElementHandler::selectedItem(void)
{
  if (!hasSelected()) {
    return nullptr;
  }
  return static_cast<ElementWidget*>(ui->listWidget->item(ui->listWidget->currentRow()));
}


ElementHandler::ElementHandler(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ElementHandler)
{
  ui->setupUi(this);
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
    ElementWidget* item = itemFromElement(element);
    ui->listWidget->addItem(item);
  }
  qDebug() << " ############################### SETTING ELEMENTS: " << ui->listWidget->count();
}

void
ElementHandler::clear(void)
{
  qDebug() << " ############################### CLEARING " << ui->listWidget->count();
  while (ui->listWidget->count() > 0) {
    freeItem(static_cast<ElementWidget*>(ui->listWidget->takeItem(0)));
  }
  qDebug() << " ############################### AFTER: CLEARING " << ui->listWidget->count();
}

bool
ElementHandler::selectNext(void)
{
  const int curr_row = ui->listWidget->currentRow();
  ui->listWidget->setCurrentRow((curr_row + 1) >= ui->listWidget->count() ? 0 : curr_row + 1);
  return true;
}

bool
ElementHandler::selectPrev(void)
{
  const int curr_row = ui->listWidget->currentRow();
  ui->listWidget->setCurrentRow(curr_row <= 0 ? ui->listWidget->count() - 1 : curr_row - 1);
  return true;
}

void
ElementHandler::unselectCurrent(void)
{
  ui->listWidget->setCurrentRow(-1);
}

bool
ElementHandler::hasSelected(void) const
{
  return ui->listWidget->currentRow() >= 0;
}

Element::ConstPtr
ElementHandler::selected(void)
{
  ElementWidget* widget = selectedItem();
  return widget == nullptr ? nullptr : widget->element();
}

