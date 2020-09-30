#ifndef QT_CLIENT_CONTENT_TABLE_WIDGET_ITEM_H_
#define QT_CLIENT_CONTENT_TABLE_WIDGET_ITEM_H_

#include <QTableWidgetItem>

#include <data/content.h>


namespace qt_client {

class ContentTableWidgetItem : public QTableWidgetItem
{
  public:
    ContentTableWidgetItem();
    virtual ~ContentTableWidgetItem();

    /**
     * @brief setContent
     * @param content
     */
    void
    setContent(data::Content::ConstPtr content);
    data::Content::ConstPtr
    content() const;



  private:
    data::Content::ConstPtr content_;
};



} // namespace qt_client

#endif // QT_CLIENT_CONTENT_TABLE_WIDGET_ITEM_H_
