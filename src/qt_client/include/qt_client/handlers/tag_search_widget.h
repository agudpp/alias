///
/// TODO: we need to change this file to be the main search logic, is not only for tags
///       but for everything
///

#ifndef QT_CLIENT_TAG_SEARCH_WIDGET_H_
#define QT_CLIENT_TAG_SEARCH_WIDGET_H_

#include <vector>

#include <QWidget>

#include <service/service_api.h>

#include <qt_client/common/function_key_trigger.h>
#include <qt_client/common/widget_line_edit.h>
#include <qt_client/data_widgets/tag_list_widget.h>
#include <qt_client/data_widgets/tag_suggestion_list_widget.h>
#include <qt_client/handlers/tag_logic_handler.h>
#include <qt_client/content/content_table_widget.h>
#include <qt_client/handlers/content_editor_widget.h>


namespace Ui {
class TagSearchWidget;
}


namespace qt_client {

class TagSearchWidget : public QWidget
{
    Q_OBJECT

  public:
    TagSearchWidget(QWidget* parent = nullptr,
                    service::ServiceAPI::Ptr service_api = nullptr);
    virtual ~TagSearchWidget();


    /**
     * @brief activate
     */
    void
    activate();

    /**
     * @brief clearAll
     */
    void
    clearAll();

  signals:

    /**
     * @brief This signal will be emitted when the user already finished operating with this
     */
    void
    usageDone();


  private slots:

    /**
     * @brief When the highlighting of a tag changed
     */
    void
    onSuggestedTagHighlightingChanged();

    /**
     * @brief When a suggested tag is selected
     */
    void
    onSuggestedTagSelected(TagWidget*);

    /**
     * @brief When a tag of the already selected ones is removed
     */
    void
    onTagRemoved(TagWidget*);

    /**
     * @brief Whenever the input text has changed
     * @param text
     */
    void
    onInputTextChanged(const QString& text);

    /**
     * @brief Slot to process unhandled key events from the input of the tag logic handler
     * @param key_event The key event
     */
    void
    onTagInputUnhandledKeyEvent(QKeyEvent* key_event);


  private:

    /**
     * @brief Updates all the ui elements based on the current search query / state and result
     * @param search_context  The search state performed
     * @param tag_result      The results of the tag search
     */
    void
    updateTagUI(const service::SearchContext& search_context,
                const service::TagSearchReslut& tag_result);

    /**
     * @brief Updates the content ui based on the latest search and results
     * @param content_result  The content search result
     */
    void
    updateContentUI(const std::set<data::Content::ConstPtr>& content_result);

    /**
     * @brief Will perform the search on the api based on the current search context
     *        and update all the required ui elements
     * @param search_context The current search context
     */
    void
    performSearch(const service::SearchContext& search_context);

    /**
     * @brief Add a simple key trigger to the list
     * @param key the key
     * @param type the type
     */
    void
    addSimpleKeyTrigger(Qt::Key key, QEvent::Type type, bool (TagSearchWidget::* fun)(QKeyEvent* key_event));

    //
    // The list of actions we are handling at this level
    //
    bool
    selectNextContentAction();
    bool
    selectPrevContentAction();
    bool
    processContentAction(const data::Content::ConstPtr& content);
    bool
    editContentAction(data::Content::Ptr content);
    bool
    createContentAction();
    bool
    onDoneAction();

    /**
     * @brief onUpKeyReleased
     * @param key_event
     */
    bool
    onUpKeyReleased(QKeyEvent* key_event);
    bool
    onDownKeyReleased(QKeyEvent* key_event);
    bool
    onReturnKeyReleased(QKeyEvent* key_event);
    bool
    onEscapeKeyReleased(QKeyEvent* key_event);


  private:
    Ui::TagSearchWidget *ui;
    TagListWidget* tag_list_widget_;
    TagSuggestionListWidget* tag_suggested_widget_;
    WidgetLineEdit* widget_line_edit_;
    TagLogicHandler* tag_logic_handler_;
    std::vector<TagWidget*> tag_widgets_;
    // key triggers
    std::vector<KeyTrigger::Ptr> key_triggers_;
    // content part
    ContentEditorWidget* editor_widget_;
    ContentTableWidget* content_table_widget_;
    // service api objects
    service::ServiceAPI::Ptr service_api_;
    service::SearchContext search_context_;
    service::ContentSearchResult content_search_last_result_;
};

} // namespace qt_client

#endif // QT_CLIENT_TAG_SEARCH_WIDGET_H_
