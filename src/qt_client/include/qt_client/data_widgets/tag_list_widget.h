#ifndef QT_CLIENT_TAG_LIST_HANDLER_H_
#define QT_CLIENT_TAG_LIST_HANDLER_H_

#include <vector>

#include <QWidget>

#include <qt_client/data_widgets/tag_widget.h>


namespace Ui {
class TagListWidget;
}


namespace qt_client {

class TagListWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TagListWidget(QWidget *parent = nullptr);
    ~TagListWidget();

    /**
     * @brief pushTag, the caller is the owner of the memory
     * @param tag
     */
    void
    pushTag(TagWidget* tag);

    /**
     * @brief popTag, caller should handle the memory
     * @return the tag being popped
     */
    TagWidget*
    popTag();

    /**
     * @brief push a list of Tags
     * @param tags
     */
    void
    pushTags(const std::vector<TagWidget*>& tags);

    /**
     * @brief This will pop all the tags, caller should handle the memory
     */
    std::vector<TagWidget*>
    popAllTags();

    /**
     * @brief tags
     * @return
     */
    inline const std::vector<TagWidget*>&
    tags(void) const
    {
      return tags_;
    }

    inline bool
    hasTags() const
    {
      return !tags_.empty();
    }

  private:

    /**
     * @brief updateSize
     */
    void
    updateSize();

  private:
    Ui::TagListWidget *ui;
    std::vector<TagWidget*> tags_;
};


} // namespace qt_client

#endif // QT_CLIENT_TAG_LIST_HANDLER_H_

