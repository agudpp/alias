#include <ui_client/elements/editors/simple_text_editor.h>

#include <QKeyEvent>

#include <core/debug/Debug.h>
#include <elements/simple_text_element.h>

#include "ui_simple_text_editor.h"


SimpleTextEditor::SimpleTextEditor(QWidget *parent) :
  ElementEditorInterface(parent),
  ui(new Ui::SimpleTextEditor)
{
  ui->setupUi(this);
}

SimpleTextEditor::~SimpleTextEditor()
{
  delete ui;
}

void
SimpleTextEditor::saveEdition(void)
{
  dynamic_cast<SimpleTextElement*>(element_.get())->setText(ui->plainTextEdit->toPlainText().toStdString());
}

void
SimpleTextEditor::prepareEdition(void)
{
  ASSERT_PTR(element_.get());

  ui->plainTextEdit->setPlainText(dynamic_cast<SimpleTextElement*>(element_.get())->text().c_str());
}
