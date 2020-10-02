#include <qt_client/handlers/tagger_widget.h>

#include <QObject>

#include <toolbox/debug/debug.h>

#include "ui_tagger_widget.h"


namespace qt_client {


void
TaggerWidget::onSuggestedTagHighlightingChanged()
{
  // nothing to do...
}

void
TaggerWidget::onSuggestedTagSelected(TagWidget* tag)
{
  ASSERT_PTR(tag);
  LOG_INFO("Tag added " << tag->tag());
  auto selected_tag = getSelectedTagByName(tag->tag()->name());
  if (selected_tag.get() == nullptr) {
    selected_tags_.push_back(tag->tag());
  }
  performSearch(service::SearchContext());
}

void
TaggerWidget::onTagRemoved(TagWidget* tag)
{
  ASSERT_PTR(tag);
  LOG_INFO("Tag removed " << tag->tag());
  selected_tags_.erase(std::remove(selected_tags_.begin(), selected_tags_.end(), tag->tag()), selected_tags_.end());
  performSearch(service::SearchContext());
}

void
TaggerWidget::onInputTextChanged(const QString& text)
{
  service::SearchContext search_context;
  search_context.query = text.toStdString();
  performSearch(search_context);
}

void
TaggerWidget::onTagCreationIntention(const QString& tag_text)
{
  const std::string tag_text_str = tag_text.toStdString();
  data::Tag::ConstPtr tag = getSelectedTagByName(tag_text_str);
  if (tag.get() == nullptr) {
    if (!service_api_->getTagByName(tag_text_str, tag)) {
      tag.reset(new data::Tag(tag_text.toStdString()));
    }
    selected_tags_.push_back(tag);
  }

  performSearch(service::SearchContext(), true);
}

void
TaggerWidget::updateTagUI(const service::TagSearchReslut& tag_result, bool clear_input)
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
  std::set<data::Tag::ConstPtr> selected_tag_set;
  for (auto& curr_tag_ptr : selected_tags_) {
    TagWidget* tag_widget_ptr(new TagWidget(nullptr, curr_tag_ptr));
    tag_widgets_.push_back(tag_widget_ptr);
    selected_tag_set.insert(curr_tag_ptr);
    current_tags.push_back(tag_widget_ptr);
  }

  std::vector<TagWidget*> suggested_tags;
  for (auto& exp_tag_ptr : tag_result.expanded_tags) {
    if (selected_tag_set.count(exp_tag_ptr) > 0) {
      continue;
    }
    TagWidget* tag_widget_ptr(new TagWidget(nullptr, exp_tag_ptr));
    tag_widgets_.push_back(tag_widget_ptr);
    suggested_tags.push_back(tag_widget_ptr);
  }

  tag_logic_handler_->configure(current_tags, suggested_tags, clear_input);
}

void
TaggerWidget::performSearch(const service::SearchContext& search_context, bool clear_input)
{
  service::TagSearchReslut tag_result;
  if (!service_api_->searchTags(search_context, tag_result)) {
    LOG_ERROR("Something happened when trying to perform the search");
    return;
  }
  updateTagUI(tag_result, clear_input);
}

data::Tag::ConstPtr
TaggerWidget::getSelectedTagByName(const std::string& tag_name) const
{
  for (auto& tag : selected_tags_) {
    if (tag->name() == tag_name) {
      return tag;
    }
  }
  return nullptr;
}


TaggerWidget::TaggerWidget(QWidget* parent,
                           service::ServiceAPI::Ptr service_api) :
  QWidget(parent),
  ui(new Ui::TaggerWidget)
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
  delete ui->lineEdit;

  ui->verticalLayout->addWidget(widget_line_edit_);
  ui->verticalLayout->addWidget(tag_suggested_widget_);

  tag_logic_handler_ = new TagLogicHandler(tag_list_widget_, tag_suggested_widget_, widget_line_edit_);

  // connect signals
  QObject::connect(tag_logic_handler_, &TagLogicHandler::suggestedTagSelected,
                   this, &TaggerWidget::onSuggestedTagSelected);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::suggestedTagHighlightingChanged,
                   this, &TaggerWidget::onSuggestedTagHighlightingChanged);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::tagRemoved,
                   this, &TaggerWidget::onTagRemoved);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::inputTextChanged,
                   this, &TaggerWidget::onInputTextChanged);
  QObject::connect(tag_logic_handler_, &TagLogicHandler::tagCreationIntention,
                   this, &TaggerWidget::onTagCreationIntention);
}

TaggerWidget::~TaggerWidget()
{
  delete ui;
  clearAll();

  delete tag_logic_handler_;
}

void
TaggerWidget::setSelectedTags(const std::vector<data::Tag::ConstPtr>& tags)
{
  selected_tags_ = tags;
  performSearch(service::SearchContext(), true);
}

const std::vector<data::Tag::ConstPtr>&
TaggerWidget::selectedTags() const
{
  return selected_tags_;
}

void
TaggerWidget::clearAll()
{
  tag_list_widget_->popAllTags();
  tag_suggested_widget_->popAllTags();
  for (auto& tag : tag_widgets_) {
    delete tag;
  }
  tag_widgets_.clear();
}



} // namespace qt_client

