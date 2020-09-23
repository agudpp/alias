#ifndef QT_CLIENT_TAG_SUGGESTION_LIST_HANDLER_H_
#define QT_CLIENT_TAG_SUGGESTION_LIST_HANDLER_H_

#include <vector>

#include <QWidget>

#include <qt_client/data_widgets/tag_widget.h>


namespace Ui {
class TagSuggestionListWidget;
}


namespace qt_client {

class TagSuggestionListWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TagSuggestionListWidget(QWidget *parent = nullptr);
    ~TagSuggestionListWidget();

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
     * @brief Remove a tag from the list, unhighlights all
     * @param tag The tag to remove
     * @return true if we removed it, false otherwise
     */
    bool
    removeTag(TagWidget* tag);

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

    /**
     * @brief first tag of the list
     * @return first tag of the list
     */
    TagWidget*
    first() const;

    /**
     * @brief last tag of the list
     * @return last tag of the list
     */
    TagWidget*
    last() const;

    /**
     * @brief current highlihgt tag
     * @return highlighted tag
     */
    TagWidget*
    currentHighlighted() const;

    /**
     * @brief Check if we have highlighted a tag
     * @return true if we have, false otherwise
     */
    bool
    hasHighlighted() const;

    /**
     * @brief next tag from the current highlighted
     * @return next tag from the current highlighted
     */
    TagWidget*
    next() const;

    /**
     * @brief prev tag from the current highlighted
     * @return prev tag from the current highlighted
     */
    TagWidget*
    prev() const;

    /**
     * @brief highlight
     * @param tag
     */
    void
    highlight(TagWidget* tag);

    /**
     * @brief unhighlightAll
     */
    void
    unhighlightAll();


  private:

    /**
     * @brief index from a tag
     * @param tag the tag to search
     * @return the index if found or -1 if not
     */
    int
    index(const TagWidget* tag) const;

    /**
     * @brief Checks if the given index is valid within the range of tags vector
     * @param index the index to check
     * @return true if it is | false otherwise
     */
    bool
    isValidIndex(int index) const;


  private:
    Ui::TagSuggestionListWidget *ui;
    std::vector<TagWidget*> tags_;
    int current_;
};


} // namespace qt_client

#endif // QT_CLIENT_TAG_SUGGESTION_LIST_HANDLER_H_

