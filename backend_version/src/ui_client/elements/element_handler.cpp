#include "element_handler.h"
#include "ui_element_handler.h"



ElementWidget*
ElementHandler::itemFromElement(Element::ConstPtr& element)
{
  ElementWidget* result = new ElementWidget();
  result->setElement(element);
  return result;
}


ElementHandler::ElementHandler(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::ElementHandler)
{
  ui->setupUi(this);
}

ElementHandler::~ElementHandler()
{
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
}

void
ElementHandler::clear(void)
{
  ui->listWidget->clear();
}

bool
ElementHandler::selectNext(void)
{
  ui->listWidget->setCurrentRow(ui->listWidget->currentRow() + 1);
}

bool
ElementHandler::selectPrev(void)
{
  ui->listWidget->setCurrentRow(ui->listWidget->currentRow() - 1);
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

bool
ElementHandler::executeSelected(void)
{

}

bool
ElementHandler::editSelected(void)
{

}

