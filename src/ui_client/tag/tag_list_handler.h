#ifndef TAG_LIST_HANDLER_H
#define TAG_LIST_HANDLER_H

#include <vector>

#include <QWidget>

#include <ui_client/tag/tag_widget.h>


namespace Ui {
class TagListHandler;
}

class TagListHandler : public QWidget
{
    Q_OBJECT

  public:
    explicit TagListHandler(QWidget *parent = nullptr);
    ~TagListHandler();

    void
    addTag(TagWidget* tag);

    void
    popTag(TagWidget* tag);

    void
    setTags(const std::vector<TagWidget*>& tags);

    /**
     * @brief Checks if there is a tag with a given text
     * @param text the text
     * @return true if it is, false otherwisr
     */
    bool
    hasTagWithText(const std::string& text) const;

    /**
     * @brief This will pop all the tgs into the given vector
     * @param tags the tags to be filled in
     */
    void
    popAllTags(std::vector<TagWidget*>& tags);

    inline const std::vector<TagWidget*>&
    tags(void) const
    {
      return tags_;
    }

    void
    select(TagWidget* tag);

    void
    unselect(TagWidget* tag);

    TagWidget*
    first(void);

    TagWidget*
    last(void);

    bool
    selectNext(void);

    bool
    selectPrev(void);

    bool
    hasSelection(void) const;

    TagWidget*
    selected(void) const;

    void
    clear(void);

    bool
    hasTags(void) const;

  private:

    std::size_t
    indexOf(const TagWidget* tag) const;

    inline TagWidget*
    byIndex(int index) const
    {
      return (index >= 0 && index < int(tags_.size())) ? tags_[std::size_t(index)] : nullptr;
    }

  private:
    Ui::TagListHandler *ui;
    std::vector<TagWidget*> tags_;
    int index_;


};

#endif // TAG_LIST_HANDLER_H
