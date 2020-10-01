#ifndef QT_CLIENT_CONVERTER_UTILS_H_
#define QT_CLIENT_CONVERTER_UTILS_H_

#include <vector>
#include <set>

#include <data/tag.h>
#include <data/content.h>

#include <qt_client/data_widgets/tag_widget.h>


namespace qt_client {

class ConverterUtils
{
  public:

    /**
     * @brief toTextVec
     * @param tags
     * @return
     */
    static std::vector<std::string>
    toTextVec(const std::set<data::Tag::ConstPtr>& tags);
    static std::vector<std::string>
    toTextVec(const std::vector<TagWidget*>& tags);

    /**
     * @brief Convert a list of tags into a list of ids
     * @param tags  The tags to get their ids
     * @return the list of the tag ids
     */
    static std::set<toolbox::UID>
    toIdsSet(const std::vector<data::Tag::ConstPtr>& tags);

    static std::vector<data::Content::ConstPtr>
    toVec(const std::set<data::Content::ConstPtr>& contents);
};

} // namespace qt_client

#endif // QT_CLIENT_CONVERTER_UTILS_H_
