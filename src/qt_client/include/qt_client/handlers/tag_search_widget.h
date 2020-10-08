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

struct SessionData;

class TagSearchWidget : public QWidget
{
    Q_OBJECT

  public:
    TagSearchWidget(QWidget* parent = nullptr,
                    service::ServiceAPI::Ptr service_api = nullptr,
                    SessionData* session_data = nullptr);
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

    /**
     * @brief When the user is trying to save a content with the given data
     * @param content_data  The content data
     */
    void
    onContentSaved(const ContentEditorWidget::ContentData& content_data);


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
     * @brief Will get or create a list of tags on the backend from a given list of tags
     * @param tags  The current tags we want to check if we need to create or get on the BE
     * @return the list of all tags with the proper ids from the backend
     */
    std::vector<data::Tag::ConstPtr>
    getOrStoreTags(const std::vector<data::Tag::ConstPtr>& tags);

    /**
     * @brief Will handle the encryption associated to the provided content and return
     *        a new content with the data encrypted or not depending on the current user input
     * @param data            The plain data we want to encrypt
     * @param encrypted_data  The decrypted data if success
     * @return true if we were able to encrypt, false otherwise
     */
    bool
    handleEncryption(const std::string& data, std::string& encrypted_data);

    /**
     * @brief Will transform a encrypted content into a decrypted one handling the user input
     * @param encrypted_data  The encrypted data to decrypt
     * @param data            The decrypted data if success
     * @return true if we were able to decrypt, false otherwise
     */
    bool
    handleDecryption(const std::string& encrypted_data, std::string& data);

    /**
     * @brief Ask the user for the paraphrase
     * @param paraphrase The resulting paraphrase inserted by the user
     * @return if the user set a paraphrase, false otherwise
     */
    bool
    getParaphrase(QString& paraphrase);

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
    // session global data
    SessionData* session_data_;
};

} // namespace qt_client

#endif // QT_CLIENT_TAG_SEARCH_WIDGET_H_
