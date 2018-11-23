#include "tag_widget.h"

#include <QString>

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
