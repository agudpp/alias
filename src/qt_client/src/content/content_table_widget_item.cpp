#include <qt_client/content/content_table_widget_item.h>

namespace qt_client {

ContentTableWidgetItem::ContentTableWidgetItem()
{

}

ContentTableWidgetItem::~ContentTableWidgetItem()
{

}



void
ContentTableWidgetItem::setContent(data::Content::ConstPtr content)
{
  content_ = content;
  const QString preview_data = QString::fromStdString(content->data());
  // TODO: do not use magic numbers
  setText(preview_data.mid(0, 128));
}

data::Content::ConstPtr
ContentTableWidgetItem::content() const
{
  return content_;
}


} // namespace qt_client
