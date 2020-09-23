#include <qt_client/data_widgets/tag_suggestion_list_widget.h>

#include <toolbox/debug/debug.h>

#include "ui_tag_suggestion_list_widget.h"



namespace qt_client {

int
TagSuggestionListWidget::index(const TagWidget* tag) const
{
  for (unsigned int i = 0; i < tags_.size(); ++i) {
    if (tags_[i] == tag) {
      return int(i);
    }
  }
  return -1;
}

bool
TagSuggestionListWidget::isValidIndex(int index) const
{
  return index >= 0 && std::size_t(index) < tags_.size();
}


TagSuggestionListWidget::TagSuggestionListWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TagSuggestionListWidget),
  current_(-1)
{
  ui->setupUi(this);
  setStyleSheet("border:2px solid black;");
}

TagSuggestionListWidget::~TagSuggestionListWidget()
{
  delete ui;
}

void
TagSuggestionListWidget::pushTag(TagWidget* tag)
{
  ASSERT_PTR(tag);
  tags_.push_back(tag);
  ui->horizontalLayout->insertWidget(ui->horizontalLayout->count() - 1, tag);
}

TagWidget*
TagSuggestionListWidget::popTag()
{
  TagWidget* last = tags_.back();
  tags_.pop_back();
  ui->horizontalLayout->removeWidget(last);
  return last;
}

bool
TagSuggestionListWidget::removeTag(TagWidget* tag)
{
  const int tag_index = index(tag);
  if (!isValidIndex(tag_index)) {
    return false;
  }
  unhighlightAll();
  ui->horizontalLayout->removeWidget(tag);
  tags_.erase(tags_.begin() + tag_index);
  return true;
}

void
TagSuggestionListWidget::pushTags(const std::vector<TagWidget*>& tags)
{
  for (TagWidget* t : tags) {
    pushTag(t);
  }
}

std::vector<TagWidget*>
TagSuggestionListWidget::popAllTags()
{
  std::vector<TagWidget*> result = tags_;
  for (TagWidget* t : tags_) {
    ui->horizontalLayout->removeWidget(t);
  }
  tags_.clear();
  return result;
}

TagWidget*
TagSuggestionListWidget::first() const
{
  return tags_.empty() ? nullptr : tags_.front();
}

TagWidget*
TagSuggestionListWidget::last() const
{
  return tags_.empty() ? nullptr : tags_.back();
}

TagWidget*
TagSuggestionListWidget::currentHighlighted() const
{
  return isValidIndex(current_) ? tags_[current_] : nullptr;
}

bool
TagSuggestionListWidget::hasHighlighted() const
{
  return currentHighlighted() != nullptr;
}

TagWidget*
TagSuggestionListWidget::next() const
{
  if (tags_.empty()) {
    return nullptr;
  }
  const int next_index = (current_ + 1) % tags_.size();
  return tags_[next_index];
}

TagWidget*
TagSuggestionListWidget::prev() const
{
  if (tags_.empty()) {
    return nullptr;
  }
  const int prev_index = (current_ == 0) ? (tags_.size() - 1) : (current_ - 1);
  return tags_[prev_index];
}

void
TagSuggestionListWidget::highlight(TagWidget* tag)
{
  const int tag_index = index(tag);
  if (!isValidIndex(tag_index)) {
    return;
  }
  unhighlightAll();
  current_ = tag_index;
  tags_[current_]->highlight();
}

void
TagSuggestionListWidget::unhighlightAll()
{
  for (auto& tag : tags_) {
    tag->unhighlight();
  }
  current_ = -1;
}


} // namespace qt_client
