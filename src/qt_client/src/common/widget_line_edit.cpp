#include <qt_client/common/widget_line_edit.h>

#include <QToolButton>
#include <QStyle>
#include <QEvent>
#include <QResizeEvent>
#include <QObject>

#include <toolbox/debug/debug.h>


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
  widget_->installEventFilter(this);
}

bool
WidgetLineEdit::eventFilter(QObject *object, QEvent *event)
{
  if (object == widget_ && event->type() == QEvent::Resize) {
    adjustSize(static_cast<QResizeEvent*>(event)->size());
  }

  return false;
}

void
WidgetLineEdit::adjustSize(const QSize& sz)
{
  widget_->move(0, rect().height()/2 - sz.height()/2);
  QMargins text_margins = textMargins();
  text_margins.setLeft(sz.width() + 1);
  setTextMargins(text_margins);
}

void
WidgetLineEdit::resizeEvent(QResizeEvent *event)
{
  adjustSize(widget_->size());
  QWidget::resizeEvent(event);
}



} // namespace qt_client

