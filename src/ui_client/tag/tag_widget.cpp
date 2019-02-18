#include <ui_client/tag/tag_widget.h>

#include <QString>
#include <QtDebug>

#include "ui_tag_widget.h"


static const QString BASE_STYLE = "border:2px solid grey; \nborder-radius: 5px;";



TagWidget::TagWidget(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::TagWidget)
{
  ui->setupUi(this);
}

TagWidget::~TagWidget()
{
  delete ui;
}

void
TagWidget::configure(const Tag::ConstPtr& tag)
{
  if (tag.get() == nullptr) {
    return;
  }
  ui->label->setText(tag->text().c_str());
  tag_ = tag;
}


void
TagWidget::highlight(void)
{
  setStyleSheet(BASE_STYLE + "background-color: black; color: white");
  qDebug() << "Highlightingggg ";
}

void
TagWidget::unhighlight(void)
{
  setStyleSheet(BASE_STYLE + "color: black");
}

void
TagWidget::clear(void)
{
  unhighlight();
  tag_.reset();
}
