#include "element_widget.h"



ElementWidget::ElementWidget(QListWidget *parent) :
  QListWidgetItem(parent)
{

}

ElementWidget::~ElementWidget()
{

}


void
ElementWidget::setElement(Element::ConstPtr& element)
{
  element_ = element;
  setText(element->getIndexingTest().c_str());
}
