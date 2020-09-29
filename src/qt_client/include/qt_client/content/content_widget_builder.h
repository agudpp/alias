#ifndef QT_CLIENT_CONTENT_WIDGET_BUILDER_H_
#define QT_CLIENT_CONTENT_WIDGET_BUILDER_H_

#include <qt_client/content/content_widget_interface.h>

#include <data/content.h>


namespace qt_client {


class ContentWidgetBuilder
{
  public:

    /**
     * @brief Builds a read only widget for the given content ref
     * @param ref the content ref
     * @return a pointer of the associated ContentWidget on success | nullptr on error
     */
    static ContentWidgetInterface*
    buildReadOnly(data::Content::ConstPtr ref);

    /**
     * @brief Builds a editable widget for the given content ref
     * @param ref the content ref
     * @return a pointer of the associated ContentWidget on success | nullptr on error
     */
    static ContentWidgetInterface*
    buildEditable(data::Content::Ptr ref);

};

} // namespace qt_client

#endif // QT_CLIENT_CONTENT_WIDGET_BUILDER_H_
