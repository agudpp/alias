#include <qt_client/handlers/tag_search_widget.h>

#include <QObject>

#include <toolbox/debug/debug.h>
#include <toolbox/utils/std_utils.h>
#include <qt_client/common/converter_utils.h>
#include <qt_client/content/content_processor.h>


#include "ui_tag_search_widget.h"


namespace qt_client {


void
TagSearchWidget::onSuggestedTagHighlightingChanged()
{
  if (tag_suggested_widget_->hasHighlighted()) {
    TagWidget* tag_widget = tag_suggested_widget_->currentHighlighted();
    ASSERT_PTR(tag_widget);
    auto itr = content_search_last_result_.exp_results.find(tag_widget->tag());
    if (itr != content_search_last_result_.exp_results.end()) {
      updateContentUI(itr->second);
    }
  }
}

void
TagSearchWidget::onSuggestedTagSelected(TagWidget* tag)
{
  ASSERT_PTR(tag);
  LOG_INFO("Tag added " << tag->tag());
  const bool has_element = toolbox::StdUtils::contains(search_context_.tags, tag->tag());
  if (!has_element) {
    search_context_.tags.push_back(tag->tag());
    performSearch(search_context_);
  }
}

void
TagSearchWidget::onTagRemoved(TagWidget* tag)
{
  ASSERT_PTR(tag);
  LOG_INFO("Tag removed " << tag->tag());
  const bool has_element = toolbox::StdUtils::contains(search_context_.tags, tag->tag());
  if (has_element) {
    toolbox::StdUtils::remove(search_context_.tags, tag->tag());
    performSearch(search_context_);
  }
}

void
TagSearchWidget::onInputTextChanged(const QString& text)
{
  search_context_.query = text.toStdString();
  performSearch(search_context_);
}

void
TagSearchWidget::onTagInputUnhandledKeyEvent(QKeyEvent* key_event)
{
  for (KeyTrigger::Ptr& kt : key_triggers_) {
    if (kt->shouldTrigger(key_event)) {
      if (kt->trigger(key_event)) {
        return;
      }
    }
  }
}

void
TagSearchWidget::updateTagUI(const service::SearchContext& search_context,
                             const service::TagSearchReslut& tag_result)
{
  // TODO: improve tags memory handling here, note that we are keeping all the tags
  //       on a separate vector to reuse them
  tag_list_widget_->popAllTags();
  tag_suggested_widget_->popAllTags();
  while (!tag_widgets_.empty()) {
    delete tag_widgets_.back();
    tag_widgets_.pop_back();
  }

  std::vector<TagWidget*> current_tags;
  for (auto& curr_tag_ptr : search_context.tags) {
    TagWidget* tag_widget_ptr(new TagWidget(nullptr, curr_tag_ptr, true));
    tag_widgets_.push_back(tag_widget_ptr);
    current_tags.push_back(tag_widget_ptr);
  }

  std::vector<TagWidget*> suggested_tags;
  for (auto& exp_tag_ptr : tag_result.expanded_tags) {
    TagWidget* tag_widget_ptr(new TagWidget(nullptr, exp_tag_ptr));
    tag_widgets_.push_back(tag_widget_ptr);
    suggested_tags.push_back(tag_widget_ptr);
  }

  tag_logic_handler_->configure(current_tags, suggested_tags);
}

void
TagSearchWidget::updateContentUI(const std::set<data::Content::ConstPtr>& content_result)
{
  content_table_widget_->clear();
  content_table_widget_->addContents(ConverterUtils::toVec(content_result));
}

void
TagSearchWidget::performSearch(const service::SearchContext& search_context)
{
  service::TagSearchReslut tag_result;

  if (!service_api_->searchTags(search_context, tag_result)) {
    LOG_ERROR("Something happened when trying to perform the search");
    return;
  }

  if (!service_api_->searchContent(search_context, content_search_last_result_)) {
    LOG_ERROR("Something happened when searching for the content information");
  }

  updateTagUI(search_context, tag_result);
  updateContentUI(content_search_last_result_.tagged_contents);
}

void
TagSearchWidget::addSimpleKeyTrigger(Qt::Key key, QEvent::Type type, bool (TagSearchWidget::* fun)(QKeyEvent* key_event))
{
  KeyTrigger::Configuration config(key);
  config.event_type = type;
  FunctionKeyTrigger* key_trigger = new FunctionKeyTrigger(config, std::bind(fun, this, std::placeholders::_1));
  key_triggers_.push_back(KeyTrigger::Ptr(key_trigger));
}

bool
TagSearchWidget::selectNextContentAction()
{
  content_table_widget_->selectNext();
  return true;
}

bool
TagSearchWidget::selectPrevContentAction()
{
  content_table_widget_->selectPrev();
  return true;
}

bool
TagSearchWidget::processContentAction(const data::Content::ConstPtr& content)
{
  LOG_INFO("Selected content: " << content->data());
  if (!ContentProcessor::process(content)) {
    LOG_ERROR("Error processing the content");
    return false;
  }

  return onDoneAction();
}

bool
TagSearchWidget::editContentAction(data::Content::Ptr content)
{
  editor_widget_->setEditableContent(content);
  editor_widget_->show();
  editor_widget_->activate();
  return true;
}

bool
TagSearchWidget::createContentAction()
{
  // TODO: for now we will always create a text content type
  data::Content::Ptr content(new data::Content);
  data::Metadata metadata;
  metadata.setType(data::ContentType::TEXT);
  content->setMetadata(metadata);

  editor_widget_->setEditableContent(content);
  editor_widget_->show();
  editor_widget_->activate();
  return true;
}

bool
TagSearchWidget::onDoneAction()
{
  // we finish the operation of the app here, nothing else to do
  emit usageDone();

  return true;
}

bool
TagSearchWidget::onUpKeyReleased(QKeyEvent*)
{
  return selectPrevContentAction();
}

bool
TagSearchWidget::onDownKeyReleased(QKeyEvent*)
{
  return selectNextContentAction();
}

bool
TagSearchWidget::onReturnKeyReleased(QKeyEvent* key_event)
{
  // we have 3 cases (3 actions) here:
  // - 1) Content not selected + Ctrl is pressed -> create a new content
  // - 2) Content selected + Ctrl pressed -> edit content
  // - 3) Content selected (no ctrl pressed) -> process content

  const bool ctrl_pressed = key_event->modifiers() & Qt::KeyboardModifier::ControlModifier;
  if (!content_table_widget_->hasSelection()) {
    if (ctrl_pressed) {
      // 1)
      return createContentAction();
    }
    return false;
  }

  ContentTableWidgetItem* current_content = content_table_widget_->currentSelected();
  ASSERT_PTR(current_content);

  // 2) and 3)
  if (ctrl_pressed) {
    data::Content::Ptr copy_content = current_content->content()->clonePtr(true);
    return editContentAction(copy_content);
  }

  // 3)
  return processContentAction(current_content->content());
}

bool
TagSearchWidget::onEscapeKeyReleased(QKeyEvent*)
{
  return onDoneAction();
}


TagSearchWidget::TagSearchWidget(QWidget* parent,
                                 service::ServiceAPI::Ptr service_api) :
  QWidget(parent),
  ui(new Ui::TagSearchWidget)
, tag_list_widget_(nullptr)
, tag_suggested_widget_(nullptr)
, widget_line_edit_(nullptr)
, tag_logic_handler_(nullptr)
, editor_widget_(nullptr)
, content_table_widget_(nullptr)
, service_api_(service_api)
{
  ui->setupUi(this);
  tag_list_widget_ = new TagListWidget();
  tag_suggested_widget_ = new TagSuggestionListWidget();
  widget_line_edit_ = new WidgetLineEdit(nullptr, tag_list_widget_);
  editor_widget_ = new ContentEditorWidget(nullptr, service_api_);
  content_table_widget_ = new ContentTableWidget(this);

  // copy the style and remove it from screen
  widget_line_edit_->setStyleSheet(ui->lineEdit->styleSheet());
  widget_line_edit_->setStyle(ui->lineEdit->style());
  widget_line_edit_->setFont(ui->lineEdit->font());
  widget_line_edit_->setMinimumSize(ui->lineEdit->minimumSize());
  delete ui->lineEdit;

  ui->verticalLayout->addWidget(content_table_widget_);
  ui->verticalLayout->addWidget(widget_line_edit_);
  ui->verticalLayout->addWidget(tag_suggested_widget_);

  editor_widget_->hide();

  tag_logic_handler_ = new TagLogicHandler(tag_list_widget_, tag_suggested_widget_, widget_line_edit_);

  // connect signals
  QObject::connect(tag_logic_handler_, &TagLogicHandler::suggestedTagSelected,
                   this, &TagSearchWidget::onSuggestedTagSelected);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::suggestedTagHighlightingChanged,
                   this, &TagSearchWidget::onSuggestedTagHighlightingChanged);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::tagRemoved,
                   this, &TagSearchWidget::onTagRemoved);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::inputTextChanged,
                   this, &TagSearchWidget::onInputTextChanged);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::unhandledKeyEvent,
                   this, &TagSearchWidget::onTagInputUnhandledKeyEvent);

  // key triggers
  addSimpleKeyTrigger(Qt::Key_Up, QEvent::KeyRelease, &TagSearchWidget::onUpKeyReleased);
  addSimpleKeyTrigger(Qt::Key_Down, QEvent::KeyRelease, &TagSearchWidget::onUpKeyReleased);
  addSimpleKeyTrigger(Qt::Key_Return, QEvent::KeyRelease, &TagSearchWidget::onReturnKeyReleased);
  addSimpleKeyTrigger(Qt::Key_Escape, QEvent::KeyRelease, &TagSearchWidget::onEscapeKeyReleased);
}

TagSearchWidget::~TagSearchWidget()
{
  delete ui;
  tag_list_widget_->popAllTags();
  tag_suggested_widget_->popAllTags();
  for (auto& tag : tag_widgets_) {
    delete tag;
  }
  tag_widgets_.clear();

  delete tag_logic_handler_;
  delete editor_widget_;
}

void
TagSearchWidget::activate()
{
  setFocus();
  widget_line_edit_->setFocus();
}

void
TagSearchWidget::clearAll()
{
  tag_logic_handler_->clearAll();
  for (auto& tag : tag_widgets_) {
    delete tag;
  }
  tag_widgets_.clear();
  content_table_widget_->clear();
  editor_widget_->hide();
}



} // namespace qt_client

