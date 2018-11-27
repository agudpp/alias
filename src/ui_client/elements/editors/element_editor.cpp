#include <ui_client/elements/editors/element_editor.h>

#include <QtDebug>
#include <QKeyEvent>

#include <core/debug/Debug.h>
#include <elements/simple_text_element.h>

#include <ui_client/elements/editors/simple_text_editor.h>

#include "ui_element_editor.h"


void
ElementEditor::buildUI(void)
{
  ASSERT_PTR(element_.get());
  if (element_->elementType() == "SimpleTextElement") {
    SimpleTextEditor* editor = new SimpleTextEditor;
    // TODO this should be at the editor level
    editor->setElement(element_);
    editor_ = editor;
    QObject::connect(editor, &SimpleTextEditor::editionFinished,
                     this, &ElementEditor::editionFinished);
    QObject::connect(editor, &SimpleTextEditor::editionCancelled,
                     this, &ElementEditor::editionCancelled);
  } else {
    debugERROR("Invalid element type, we do not have a editor for type: %s", element_->elementType().c_str());
    return;
  }

  ui->verticalLayout->insertWidget(0, editor_);

  // set the tags
  tag_handler_->setSelectedTags(std::set<Tag::ConstPtr>(tags_.begin(), tags_.end()));

  editor_->prepareEdition();
}

void
ElementEditor::clearUI(void)
{
  if (editor_ != nullptr) {
    delete editor_;
    editor_ = nullptr;
  }

}


void
ElementEditor::tagHandlerInputTextChanged(const QString& text)
{
  qDebug() << "ElementEditor::tagHandlerInputTextChanged called";
  performSearch(text);
}

void
ElementEditor::tagHandlerTagRemoved(Tag::ConstPtr tag)
{
  qDebug() << "ElementEditor::tagHandlerTagRemoved called";
  performSearch(tag_handler_->currentText());
}

void
ElementEditor::tagHandlerTagSelected(Tag::ConstPtr tag)
{
  qDebug() << "ElementEditor::tagHandlerTagSelected called";
  performSearch(tag_handler_->currentText());
}

void
ElementEditor::tagHandlerkeyPressed(QKeyEvent* event)
{
  // TODO customize controls here
  if (event->key() == Qt::Key_Escape) {
    editionCancelled();
  } else if (event->key() == Qt::Key_Return && event->modifiers() & Qt::ControlModifier) {
    editionFinished();
  }
}

void
ElementEditor::editionFinished(void)
{
  editor_->saveEdition();
  qDebug() << "CURRENT ELEMENT TEXT: " << element_->getIndexingTest().c_str();
  qDebug() << "EDITOR ELEMENT TEXT: " << editor_->element()->getIndexingTest().c_str();
  accept();
}

void
ElementEditor::editionCancelled(void)
{
  reject();
}

void
ElementEditor::performSearch(const QString& text)
{
  if (text.isEmpty()) {
    return;
  }
  qDebug() << "Performing tag search with: " << text;
  ServiceAPI::SearchOptions search_options;
  ServiceAPI::SearchTagsReslut results;
  search_options.query = text.toStdString();
  search_options.tags = tag_handler_->selectedTagsTexts();

  if (!service_api_->searchTags(search_options, results)) {
    debugERROR("Error performing the tag search, doing nothing");
    return;
  }
  tag_handler_->setSuggestedTags(results.expanded_tags);
}


ElementEditor::ElementEditor(QWidget *parent, ServiceAPI* service_api) :
  QDialog(parent),
  ui(new Ui::ElementEditor),
  service_api_(service_api),
  editor_(nullptr),
  tag_handler_(nullptr)
{
  ui->setupUi(this);

  setFocusPolicy(Qt::FocusPolicy::WheelFocus);
  setWindowFlags(Qt::WindowType::Dialog);
  setWindowFlags(Qt::WindowStaysOnTopHint);

  tag_handler_ = new TagHandlerWidget(nullptr, service_api);
  tag_handler_->setAddTagsFlag(true);
  ui->verticalLayout->addWidget(tag_handler_);
  QObject::connect(tag_handler_, &TagHandlerWidget::inputTextChanged,
           this, &ElementEditor::tagHandlerInputTextChanged);
  QObject::connect(tag_handler_, &TagHandlerWidget::tagRemoved,
           this, &ElementEditor::tagHandlerTagRemoved);
  QObject::connect(tag_handler_, &TagHandlerWidget::tagSelected,
           this, &ElementEditor::tagHandlerTagSelected);
  QObject::connect(tag_handler_, &TagHandlerWidget::someKeyPressed,
           this, &ElementEditor::tagHandlerkeyPressed);
}

ElementEditor::~ElementEditor()
{
  delete ui;
}

void
ElementEditor::editElement(Element::Ptr& element, const std::vector<Tag::ConstPtr>& with_tags)
{
  // TODO: remove this
  if (element.get() == nullptr) {
    element = Element::Ptr(new SimpleTextElement(core::UID::generateRandom()));
  }

  ASSERT_PTR(element.get());
  element_ = element;
  tags_ = with_tags;
}

Element::Ptr&
ElementEditor::element(void)
{
  return element_;
}

const std::vector<std::string>
ElementEditor::tagTexts(void) const
{
  return tag_handler_->selectedTagsTexts();
}


int
ElementEditor::executeEditor(void)
{
  clearUI();
  buildUI();
//  showTags();
  return exec();
}
