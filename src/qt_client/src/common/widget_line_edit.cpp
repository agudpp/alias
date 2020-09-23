#include <qt_client/common/widget_line_edit.h>

#include <QToolButton>
#include <QStyle>


namespace qt_client {


WidgetLineEdit::WidgetLineEdit(QWidget *parent, QWidget* the_widget) :
  QLineEdit(parent)
, widget_(the_widget)
{
  the_widget->setParent(this);
  int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
  setStyleSheet(QString("QLineEdit { padding-left: %1px; } ").arg(widget_->sizeHint().width() + frameWidth + 1));
  QSize msz = minimumSizeHint();
  setMinimumSize(qMax(msz.width(), widget_->sizeHint().height() + frameWidth * 2 + 2),
                 qMax(msz.height(), widget_->sizeHint().height() + frameWidth * 2 + 2));
}

void WidgetLineEdit::resizeEvent(QResizeEvent *)
{
  QSize sz = widget_->sizeHint();
  widget_->move(0, (rect().bottom() + 1 - sz.height())/2);
}



} // namespace qt_client

