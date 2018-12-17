#ifndef TAG_HANDLER_WIDGET_H
#define TAG_HANDLER_WIDGET_H

#include <set>
#include <deque>

#include <QWidget>

#include <core/types/auto_free_queue.h>

#include <service_api/serviceapi.h>

#include <ui_client/tag/tag_widget.h>
#include <ui_client/tag/tag_list_handler.h>
#include <ui_client/utils/key_trigger.h>


namespace Ui {
class TagHandlerWidget;
}

class TagHandlerWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TagHandlerWidget(QWidget *parent = nullptr, ServiceAPI* service_api = nullptr);
    ~TagHandlerWidget();

    /**
     * @brief Set the flag to see if we can add tags or not to selected if they do not
     *        exist (by pressing space we add a new tag for example)
     * @param can_add_flag true if we can create new tags , false otherwise
     */
    void
    setAddTagsFlag(bool can_add_flag);

    /**
     * @brief Set the current set of suggested tags
     * @param tags the tags
     */
    void
    setSuggestedTags(const std::set<Tag::ConstPtr>& tags);

    /**
     * @brief Set the selected tags
     * @param tags the tags
     */
    void
    setSelectedTags(const std::set<Tag::ConstPtr>& tags);

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

    /**
     * @brief Checks if there is selected tags
     * @return true if there are false otherwise
     */
    bool
    hasSelectedTags(void) const;

    /**
     * @brief Returns the current text of the input field
     * @return the input field text
     */
    QString
    currentText(void) const;

    /**
     * @brief Clear the current data
     */
    void
    clear(void);

    /**
     * @brief Activates this widget and set the proper focus
     */
    void
    activate(void);

  signals:

    void inputTextChanged(const QString& text);
    void tagRemoved(Tag::ConstPtr tag);
    void tagSelected(Tag::ConstPtr tag);
    void someKeyPressed(QKeyEvent* key);

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
     * @param emit_signal will emit the signal if there is a tag highlighted
     */
    void
    selectNextTag(TagListHandler* tag_handler,
                  bool left_dir,
                  bool only_if_selected = false,
                  bool emit_signal = true);

    /**
     * @brief Returns a new widget from the given tag
     * @param tag the tag
     * @return the widget
     */
    TagWidget*
    getWidget(Tag::ConstPtr& tag);

    /**
     * @brief Frees a widget item
     * @param widget the widget
     */
    void
    freeWidget(TagWidget* widget);

    /**
     * @brief Searches for a given tag or creates if dont exists
     * @param text the text
     * @param create_if_not_exists will create if not exists
     * @return
     */
    TagWidget*
    getOrCreateTag(const std::string& text, bool create_if_not_exists);

    /**
     * @brief Will pop all the widgets from the handler and free them
     * @param handler the handler
     */
    void
    popAndFreeWidgetsFromHandler(TagListHandler* handler);

    /**
     * @brief Will convert to tag widgets from a widget list
     * @param tags the tags
     * @return the tag widgets generated
     */
    std::vector<TagWidget*>
    toTagWidgets(const std::set<Tag::ConstPtr>& tags);

    /**
     * @brief Add a simple key trigger to the list
     * @param key the key
     * @param type the type
     */
    void
    addSimpleKeyTrigger(Qt::Key key, QEvent::Type type, bool (TagHandlerWidget::* fun)(QKeyEvent* key_event));

    /**
     * @brief Build key triggers
     */
    void
    buildKeyTriggers(void);

    /**
     * @brief Here all the keytriggers
     * @return true if we should absorve the event false otherwise
     */
    bool
    onTabPressed(QKeyEvent* key_event);
    bool
    onBackspacePressed(QKeyEvent* key_event);
    bool
    onEscapePressed(QKeyEvent* key_event);
    bool
    onReturnPressed(QKeyEvent* key_event);
    bool
    onSpacePressed(QKeyEvent* key_event);

  private:
    Ui::TagHandlerWidget *ui;
    TagListHandler* selected_tags_;
    TagListHandler* suggested_tags_;
    core::AutoFreeDequeue<TagWidget*> widgets_queue_;
    ServiceAPI* service_api_;
    bool can_add_flag_; /**< if we can add or not flags with space */
    std::vector<KeyTrigger::Ptr> key_triggers_;

};

#endif // TAG_HANDLER_WIDGET_H
