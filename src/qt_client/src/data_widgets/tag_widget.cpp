#include <qt_client/data_widgets/tag_widget.h>

#include <QString>
#include <QtDebug>

#include "ui_tag_widget.h"


static const QString BASE_STYLE = "border:2px solid grey; \n"
    "border-radius: 5px; "
    "margin-left: 1px; margin-right: 1px;";


namespace qt_client {


TagWidget::TagWidget(QWidget *parent, data::Tag::ConstPtr tag) :
  QWidget(parent)
, ui(new Ui::TagWidget)
{
  ui->setupUi(this);
  setTag(tag);
  setStyleSheet(BASE_STYLE);
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
TagWidget::highlight(void)
{
  setStyleSheet(BASE_STYLE + "background-color: black; color: white");
}

void
TagWidget::unhighlight(void)
{
  setStyleSheet(BASE_STYLE + "color: black");
}

void
TagWidget::reset(void)
{
  unhighlight();
  tag_.reset();
}

} // namespace qt_client

