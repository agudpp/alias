#ifndef QT_CLIENT_WIDGET_LINE_EDIT_H_
#define QT_CLIENT_WIDGET_LINE_EDIT_H_

#include <QLineEdit>

class QWidget;

namespace qt_client {

class WidgetLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    WidgetLineEdit(QWidget *parent = nullptr, QWidget* the_widget = nullptr);

protected:
    void resizeEvent(QResizeEvent *);

private:
    QWidget* widget_;
};

} // namespace qt_client

#endif // QT_CLIENT_WIDGET_LINE_EDIT_H_

