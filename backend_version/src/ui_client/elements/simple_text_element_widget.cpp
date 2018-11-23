#include "simple_text_element_widget.h"
#include "ui_simple_text_element_widget.h"

SimpleTextElementWidget::SimpleTextElementWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SimpleTextElementWidget)
{
  ui->setupUi(this);
}

SimpleTextElementWidget::~SimpleTextElementWidget()
{
  delete ui;
}


void
SimpleTextElementWidget::setElement(const SimpleTextElement::ConstPtr& element)
{
  if (element.get() == nullptr) {
    return;
  }
  element_ = element;
  ui->plainTextEdit->setPlainText(element->text().c_str());
}
