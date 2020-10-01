#include <qt_client/handlers/tag_search_widget.h>

#include <QObject>

#include <toolbox/debug/debug.h>
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
  auto inserted = search_context_.tags.insert(tag->tag());
  if (inserted.second) {
    performSearch(search_context_);
  }
}

void
TagSearchWidget::onTagRemoved(TagWidget* tag)
{
  ASSERT_PTR(tag);
  LOG_INFO("Tag removed " << tag->tag());
  auto removed = search_context_.tags.erase(tag->tag());
  if (removed > 0) {
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
    TagWidget* tag_widget_ptr(new TagWidget(nullptr, curr_tag_ptr));
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
TagSearchWidget::onUpKeyReleased(QKeyEvent*)
{
  content_table_widget_->selectPrev();
  return true;
}

bool
TagSearchWidget::onDownKeyReleased(QKeyEvent*)
{
  content_table_widget_->selectNext();
  return true;
}

bool
TagSearchWidget::onReturnKeyReleased(QKeyEvent*)
{
  // we only care when there are content selected on the list, otherwise we do not care
  if (!content_table_widget_->hasSelection()) {
    return false;
  }

  ContentTableWidgetItem* current_content = content_table_widget_->currentSelected();
  ASSERT_PTR(current_content);
  LOG_INFO("Selected content: " << current_content->content()->data());
  if (!ContentProcessor::process(current_content->content())) {
    LOG_ERROR("Error processing the content");
  }

  // we finish the operation of the app here, nothing else to do
  emit usageDone();

  return true;
}

bool
TagSearchWidget::onEscapeKeyReleased(QKeyEvent*)
{
  emit usageDone();
  return true;
}


TagSearchWidget::TagSearchWidget(QWidget* parent,
                                 service::ServiceAPI::Ptr service_api) :
  QWidget(parent),
  ui(new Ui::TagSearchWidget)
, tag_list_widget_(nullptr)
, tag_suggested_widget_(nullptr)
, widget_line_edit_(nullptr)
, tag_logic_handler_(nullptr)
, content_table_widget_(nullptr)
, service_api_(service_api)
{
  ui->setupUi(this);
  tag_list_widget_ = new TagListWidget();
  tag_suggested_widget_ = new TagSuggestionListWidget();
  widget_line_edit_ = new WidgetLineEdit(nullptr, tag_list_widget_);
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
}



} // namespace qt_client

