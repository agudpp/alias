///
/// TODO: we need to change this file to be the main search logic, is not only for tags
///       but for everything
///

#ifndef QT_CLIENT_TAG_SEARCH_WIDGET_H_
#define QT_CLIENT_TAG_SEARCH_WIDGET_H_

#include <vector>

#include <QWidget>

#include <service/service_api.h>

#include <qt_client/common/widget_line_edit.h>
#include <qt_client/data_widgets/tag_list_widget.h>
#include <qt_client/data_widgets/tag_suggestion_list_widget.h>
#include <qt_client/handlers/tag_logic_handler.h>


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
     * @brief Will perform the search on the api based on the current search context
     *        and update all the required ui elements
     * @param search_context The current search context
     */
    void
    performSearch(const service::SearchContext& search_context);


  private:
    Ui::TagSearchWidget *ui;
    TagListWidget* tag_list_widget_;
    TagSuggestionListWidget* tag_suggested_widget_;
    WidgetLineEdit* widget_line_edit_;
    TagLogicHandler* tag_logic_handler_;
    std::vector<TagWidget*> tag_widgets_;
    // service api objects
    service::ServiceAPI::Ptr service_api_;
    service::SearchContext search_context_;
};

} // namespace qt_client

#endif // QT_CLIENT_TAG_SEARCH_WIDGET_H_
