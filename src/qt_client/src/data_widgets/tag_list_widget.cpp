#include <qt_client/data_widgets/tag_list_widget.h>

#include <toolbox/debug/debug.h>

#include "ui_tag_list_widget.h"



namespace qt_client {


TagListWidget::TagListWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TagListWidget)
{
  ui->setupUi(this);
  resize(1,1);
}

TagListWidget::~TagListWidget()
{
  delete ui;
}

void
TagListWidget::pushTag(TagWidget* tag)
{
  ASSERT_PTR(tag);
  tags_.push_back(tag);
  ui->horizontalLayout->insertWidget(-1, tag);
  updateSize();
}

TagWidget*
TagListWidget::popTag()
{
  TagWidget* last = tags_.back();
  tags_.pop_back();
  ui->horizontalLayout->removeWidget(last);
  updateSize();
  return last;
}

void
TagListWidget::pushTags(const std::vector<TagWidget*>& tags)
{
  for (TagWidget* t : tags) {
    pushTag(t);
  }
}

std::vector<TagWidget*>
TagListWidget::popAllTags()
{
  std::vector<TagWidget*> result = tags_;
  for (TagWidget* t : tags_) {
    ui->horizontalLayout->removeWidget(t);
  }
  tags_.clear();
  updateSize();
  return result;
}


void
TagListWidget::updateSize()
{
  int w = 0;
  // TODO: we should implement this size handling in a proper way
  const int h = 84;
  for (auto& tag_widget : tags_) {
    auto tag_size_hint = tag_widget->sizeHint();
    w += tag_size_hint.width() + 2;
  }
  resize(w+1, h);
}


} // namespace qt_client
