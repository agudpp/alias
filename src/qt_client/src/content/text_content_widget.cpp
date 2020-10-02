#include <qt_client/content/text_content_widget.h>

#include <toolbox/debug/debug.h>

#include "ui_text_content_widget.h"


namespace qt_client {

TextContentWidget::TextContentWidget(QWidget* parent) :
  ContentWidgetInterface(parent)
, ui(new Ui::TextContentWidget)
{
  ui->setupUi(this);
}

TextContentWidget::~TextContentWidget()
{
  delete ui;
}

void
TextContentWidget::applyChanges()
{
  ASSERT(canEdit());
  ref()->setData(ui->plainTextEdit->toPlainText().toStdString());
}

void
TextContentWidget::activate()
{
  ui->plainTextEdit->setFocus();
}


void
TextContentWidget::newRefSet()
{
  data::Content::ConstPtr ref = anyRef();
  const QString content_text = ref.get() == nullptr ? "" : QString::fromStdString(ref->data());
  ui->plainTextEdit->setPlainText(content_text);
  LOG_INFO("Content set: " << content_text.toStdString());

  ui->plainTextEdit->setEnabled(canEdit());
}


} // namespace qt_client
