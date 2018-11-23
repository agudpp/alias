#ifndef TAG_HANDLER_WIDGET_H
#define TAG_HANDLER_WIDGET_H

#include <QWidget>

#include <ui_client/tag/tag_widget.h>
#include <ui_client/tag/tag_list_handler.h>


namespace Ui {
class TagHandlerWidget;
}

class TagHandlerWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TagHandlerWidget(QWidget *parent = nullptr);
    ~TagHandlerWidget();

    void
    setSuggestedTags(const std::vector<TagWidget*>& tags);


    bool
    eventFilter(QObject *object, QEvent *event);

    /**
     * @brief List the text of the suggested tags
     * @return the suggested tag texts
     */
    std::vector<std::string>
    suggestedTagsTexts(void) const;

    /**
     * @brief The selected tags (not highlighted but selected in the list already) texts
     * @return the sel tags texts
     */
    std::vector<std::string>
    selectedTagsTexts(void) const;

  signals:

    void inputTextChanged(const QString& text);
    void tagRemoved(TagWidget* tag);
    void tagSelected(TagWidget* tag);
    void escapePressed(void);

  private slots:

    void
    lineEditTextChanged(const QString& text);

  private:

    bool
    lineEditEventFilter(QEvent *event);

    void
    removeCurrentSelTag(void);

    /**
     * @brief Select the next tag for the given list handler in the given directoin and
     *        only if there is selected (otherwise we skip)
     * @param tag_handler the tag handler
     * @param left_dir the direction
     * @param only_if_selected if true we only select if we have selection already
     */
    void
    selectNextTag(TagListHandler* tag_handler, bool left_dir, bool only_if_selected = false);


  private:
    Ui::TagHandlerWidget *ui;
    TagListHandler* selected_tags_;
    TagListHandler* suggested_tags_;

};

#endif // TAG_HANDLER_WIDGET_H
