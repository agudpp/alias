#include <qt_client/data_widgets/tag_list_widget.h>

#include <toolbox/debug/debug.h>

#include "ui_tag_list_widget.h"



namespace qt_client {


TagListWidget::TagListWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TagListWidget)
{
  ui->setupUi(this);
  setStyleSheet("border:2px solid black;");
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
  int h = height();
  for (auto& tag_widget : tags_) {
    w += tag_widget->sizeHint().width() + 2;
    h = qMax(h, tag_widget->height());
  }
//  setMinimumWidth(w);
//  setMaximumWidth(w+1);
  resize(w+1, h);
}


} // namespace qt_client
