#include <qt_client/data_widgets/tag_widget.h>

#include <QString>
#include <QtDebug>

#include "ui_tag_widget.h"



namespace qt_client {


TagWidget::TagWidget(QWidget *parent, data::Tag::ConstPtr tag, bool show_close_button) :
  QWidget(parent)
, ui(new Ui::TagWidget)
{
  ui->setupUi(this);
  showCloseButton(show_close_button);
  setTag(tag);
}

TagWidget::~TagWidget()
{
  delete ui;
}

void
TagWidget::setTag(data::Tag::ConstPtr tag)
{
  if (tag.get() == nullptr) {
    return;
  }
  ui->label->setText(tag->name().c_str());
  tag_ = tag;
}

data::Tag::ConstPtr
TagWidget::tag()
{
  return tag_;
}

const data::Tag::ConstPtr&
TagWidget::tag() const
{
  return tag_;
}

void
TagWidget::showCloseButton(bool show)
{
  ui->close_label->setVisible(show);
}

void
TagWidget::highlight(void)
{
  setProperty("highlighted", true);
  setStyleSheet(styleSheet());
}

void
TagWidget::unhighlight(void)
{
  setProperty("highlighted", false);
  setStyleSheet(styleSheet());
}

void
TagWidget::reset(void)
{
  unhighlight();
  tag_.reset();
}

} // namespace qt_client

