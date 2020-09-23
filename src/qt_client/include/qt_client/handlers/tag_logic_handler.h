#ifndef QT_CLIENT_TAG_LOGIC_HANDLER_H_
#define QT_CLIENT_TAG_LOGIC_HANDLER_H_

#include <vector>

#include <QObject>
#include <QEvent>
#include <QKeyEvent>

#include <qt_client/common/function_key_trigger.h>
#include <qt_client/data_widgets/tag_widget.h>
#include <qt_client/data_widgets/tag_list_widget.h>
#include <qt_client/data_widgets/tag_suggestion_list_widget.h>


class QLineEdit;


namespace qt_client {

class TagLogicHandler : public QObject
{
    Q_OBJECT

  public:
    TagLogicHandler(TagListWidget* tag_list_widget,
                    TagSuggestionListWidget* tag_suggestion_widget,
                    QLineEdit* line_edit);
    virtual ~TagLogicHandler();


    /**
     * @brief eventFilter
     * @param object
     * @param event
     * @return
     */
    bool
    eventFilter(QObject *object, QEvent *event);

    /**
     * @brief Configure the tag handlers and elements based on the given tags
     * @param current_tags    The selected tags
     * @param suggested_tags  The suggested (expanding) tags
     */
    void
    configure(const std::vector<TagWidget*>& current_tags,
              const std::vector<TagWidget*>& suggested_tags);

    /**
     * @brief Remove all current data, input text, tags and all
     */
    void
    clearAll();

    /**
     * @brief Returns the suggested highlighted Tag
     * @return the suggested highlighted Tag
     */
    TagWidget*
    suggestedHighlightedTag() const;


  signals:

    /**
     * @brief When the highlighting of a tag changed
     */
    void
    suggestedTagHighlightingChanged();

    /**
     * @brief When a suggested tag is selected
     */
    void
    suggestedTagSelected(TagWidget*);

    /**
     * @brief When a tag of the already selected ones is removed
     */
    void
    tagRemoved(TagWidget*);

    /**
     * @brief Whenever the input text has changed
     * @param text
     */
    void
    inputTextChanged(const QString& text);


  private slots:

    /**
     * @brief lineEditTextChanged
     * @param text
     */
    void
    lineEditTextChanged(const QString& text);

  private:

    /**
     * @brief lineEditEventFilter
     * @param event
     * @return
     */
    bool
    lineEditEventFilter(QEvent *event);

    /**
     * @brief Add a simple key trigger to the list
     * @param key the key
     * @param type the type
     */
    void
    addSimpleKeyTrigger(Qt::Key key, QEvent::Type type, bool (TagLogicHandler::* fun)(QKeyEvent* key_event));

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
    // widgets
    TagListWidget* tag_list_widget_;
    TagSuggestionListWidget* tag_suggestion_widget_;
    QLineEdit* line_edit_;
    // internal
    std::vector<KeyTrigger::Ptr> key_triggers_;
};

} // namespace qt_client

#endif // QT_CLIENT_TAG_LOGIC_HANDLER_H_
