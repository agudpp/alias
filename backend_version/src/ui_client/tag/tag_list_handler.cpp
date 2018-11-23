#include "tag_list_handler.h"

#include <core/debug/Debug.h>

#include "ui_tag_list_handler.h"



#define IDX_OR_RET(tag) \
  const std::size_t _idx = indexOf(tag);\
  if (_idx == tags_.size()) {\
    return;\
  }\
  const int idx = static_cast<int>(_idx);



std::size_t
TagListHandler::indexOf(const TagWidget* tag) const
{
  for (std::size_t i = 0; i < tags_.size(); i++) {
    if (tag == tags_[i]) {
      return i;
    }
  }
  return tags_.size();
}



TagListHandler::TagListHandler(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TagListHandler)
{
  ui->setupUi(this);
}

TagListHandler::~TagListHandler()
{
  delete ui;
}

void
TagListHandler::addTag(TagWidget* tag)
{
  ASSERT_PTR(tag);
  tags_.push_back(tag);
  ui->horizontalLayout->addWidget(tag);
}

void
TagListHandler::removeTag(TagWidget* tag)
{
  IDX_OR_RET(tag);
  unselect(tag);
  tags_.erase(tags_.begin() + idx);
  index_--;
  if (index_ >= 0 && tags_.size() > 0) {
    select(tags_[static_cast<std::size_t>(index_)]);
  }
  ui->horizontalLayout->removeWidget(tag);
}

void
TagListHandler::setTags(const std::vector<TagWidget*>& tags)
{
  clear();
  for (TagWidget* t : tags) {
    addTag(t);
  }
}

void
TagListHandler::select(TagWidget* tag)
{
  IDX_OR_RET(tag);
  tag->highlight();
  index_ = idx;
}

void
TagListHandler::unselect(TagWidget* tag)
{
  IDX_OR_RET(tag);
  tag->unhighlight();
  index_ = -1;
}

TagWidget*
TagListHandler::first(void)
{
  return tags_.empty() ? nullptr : tags_.front();
}

TagWidget*
TagListHandler::last(void)
{
  return tags_.empty() ? nullptr : tags_.back();
}

TagWidget*
TagListHandler::selectNext(void)
{
  if (tags_.empty()) {
    return nullptr;
  }
  const int next_idx = int((index_ + 1) % tags_.size());
  unselect(selected());
  select(byIndex(next_idx));
}

TagWidget*
TagListHandler::selectPrev(void)
{
  if (tags_.empty()) {
    return nullptr;
  }
  int prev_idx = index_ - 1;
  if (prev_idx < 0) {
    prev_idx = int(tags_.size()) - 1;
  }
  unselect(selected());
  select(byIndex(prev_idx));
}

bool
TagListHandler::hasSelection(void) const
{
  return selected() != nullptr;
}

TagWidget*
TagListHandler::selected(void) const
{
  return byIndex(index_);
}

void
TagListHandler::clear(void)
{
  tags_.clear();
  index_ = -1;
}

bool
TagListHandler::hasTags(void) const
{
  return !tags_.empty();
}
