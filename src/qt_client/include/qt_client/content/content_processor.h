#ifndef QT_CLIENT_CONTENT_PROCESSOR_H_
#define QT_CLIENT_CONTENT_PROCESSOR_H_

#include <data/content.h>

namespace qt_client {


class ContentProcessor
{
  public:

    /**
     * @brief Process a content element
     * @param content The content to be processed
     * @return true on success | false otherwise
     */
    static bool
    process(const data::Content::ConstPtr& content);
};

} // namespace qt_client

#endif // QT_CLIENT_CONTENT_PROCESSOR_H_
