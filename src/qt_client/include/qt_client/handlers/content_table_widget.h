#ifndef QT_CLIENT_CONTENT_TABLE_WIDGET_H_
#define QT_CLIENT_CONTENT_TABLE_WIDGET_H_

#include <QTableWidget>

namespace qt_client {


class ContentTableWidget : public QTableWidget
{
  Q_OBJECT

  public:
    ContentTableWidget(QWidget* parent = nullptr);
    virtual ~ContentTableWidget() = default;


    // add / remove
    // select next / prev
    // get selected / has selection

};

} // namespace qt_client

#endif // QT_CLIENT_CONTENT_TABLE_WIDGET_H_
