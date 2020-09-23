#include <qt_client/handlers/tag_search_widget.h>

#include <QObject>

#include <toolbox/debug/debug.h>

#include "ui_tag_search_widget.h"


namespace qt_client {


void
TagSearchWidget::onSuggestedTagHighlightingChanged()
{

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
TagSearchWidget::performSearch(const service::SearchContext& search_context)
{
  service::TagSearchReslut tag_result;
  if (!service_api_->searchTags(search_context, tag_result)) {
    LOG_ERROR("Something happened when trying to perform the search");
    return;
  }
  updateTagUI(search_context, tag_result);
}


TagSearchWidget::TagSearchWidget(QWidget* parent,
                                 service::ServiceAPI::Ptr service_api) :
  QWidget(parent),
  ui(new Ui::TagSearchWidget)
, tag_list_widget_(nullptr)
, tag_suggested_widget_(nullptr)
, widget_line_edit_(nullptr)
, tag_logic_handler_(nullptr)
, service_api_(service_api)
{
  ui->setupUi(this);
  tag_list_widget_ = new TagListWidget();
  tag_suggested_widget_ = new TagSuggestionListWidget();
  widget_line_edit_ = new WidgetLineEdit(nullptr, tag_list_widget_);

  // copy the style and remove it from screen
  widget_line_edit_->setStyleSheet(ui->lineEdit->styleSheet());
  widget_line_edit_->setStyle(ui->lineEdit->style());
  widget_line_edit_->setFont(ui->lineEdit->font());
  widget_line_edit_->setMinimumSize(ui->lineEdit->minimumSize());
//  ui->verticalLayout->removeWidget(ui->lineEdit);
  delete ui->lineEdit;

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



} // namespace qt_client

