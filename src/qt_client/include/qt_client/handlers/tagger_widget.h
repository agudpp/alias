#ifndef QT_CLIENT_TAGGER_WIDGET_H_
#define QT_CLIENT_TAGGER_WIDGET_H_

#include <vector>

#include <QWidget>

#include <service/service_api.h>

#include <qt_client/common/widget_line_edit.h>
#include <qt_client/data_widgets/tag_list_widget.h>
#include <qt_client/data_widgets/tag_suggestion_list_widget.h>
#include <qt_client/handlers/tag_logic_handler.h>


namespace Ui {
class TaggerWidget;
}


namespace qt_client {

/**
 * @brief The TaggerWidget class will be used to search for tags without context, add new tags
 *        that does not exists, etc.
 */
class TaggerWidget : public QWidget
{
    Q_OBJECT

  public:
    TaggerWidget(QWidget* parent = nullptr,
                 service::ServiceAPI::Ptr service_api = nullptr);
    virtual ~TaggerWidget();


    /**
     * @brief Set the list of selected tags
     * @param tags the list of selected tags
     */
    void
    setSelectedTags(const std::vector<data::Tag::ConstPtr>& tags);

    /**
     * @brief Returns the list of selected tags
     * @return the list of selected tags
     */
    const std::vector<data::Tag::ConstPtr>&
    selectedTags() const;

    /**
     * @brief clearAll
     */
    void
    clearAll();


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
     * @brief If the user tries to create a tag from the input text
     * @param tag_text the tag text
     */
    void
    onTagCreationIntention(const QString& tag_text);


  private:

    /**
     * @brief Updates all the ui elements based on the selected tag and search result
     * @param tag_result      The results of the tag search
     */
    void
    updateTagUI(const service::TagSearchReslut& tag_result, bool clear_input = false);

    /**
     * @brief Will perform the search on the api based on the current search context
     *        and update all the required ui elements
     * @param search_context The current search context
     */
    void
    performSearch(const service::SearchContext& search_context, bool clear_input = false);

    /**
     * @brief getSelectedTagByName
     * @param tag_name
     * @return
     */
    data::Tag::ConstPtr
    getSelectedTagByName(const std::string& tag_name) const;


  private:
    Ui::TaggerWidget *ui;
    TagListWidget* tag_list_widget_;
    TagSuggestionListWidget* tag_suggested_widget_;
    WidgetLineEdit* widget_line_edit_;
    TagLogicHandler* tag_logic_handler_;
    std::vector<TagWidget*> tag_widgets_;
    // service api objects
    service::ServiceAPI::Ptr service_api_;
    std::vector<data::Tag::ConstPtr> selected_tags_;
};

} // namespace qt_client

#endif // QT_CLIENT_TAGGER_WIDGET_H_
