#ifndef QT_CLIENT_CONTENT_EDITOR_WIDGET_H_
#define QT_CLIENT_CONTENT_EDITOR_WIDGET_H_

#include <QWidget>

#include <service/service_api.h>

#include <data/content.h>

#include <qt_client/content/content_widget_interface.h>
#include <qt_client/handlers/tagger_widget.h>

namespace Ui {
class ContentEditorWidget;
}

namespace qt_client {

class ContentEditorWidget : public QWidget
{
  Q_OBJECT

  public:

    /**
     * @brief The ContentData struct
     */
    struct ContentData {
        data::Content::Ptr content;
        std::vector<data::Tag::ConstPtr> tags;
    };

  public:
    ContentEditorWidget(QWidget* parent = nullptr,
                        service::ServiceAPI::Ptr service_api = nullptr);
    virtual ~ContentEditorWidget();

    /**
     * @brief Set Read Only Content
     * @param content
     */
    void
    setReadOnlyContent(data::Content::ConstPtr content);

    /**
     * @brief Set Editable Content
     * @param content
     */
    void
    setEditableContent(data::Content::Ptr content);

    /**
     * @brief clearAll
     */
    void
    clearAll();

    /**
     * @brief Puts the window on top and shows the focus on the proper place
     */
    void
    activate();

  signals:

    /**
     * @brief When a new content is ready to be saved, already configured with the user changes
     * @param content_data The content to be stored.
     */
    void
    storeContent(const ContentData& content_data);

  private slots:

    /**
     * @brief onCancelClicked
     */
    void
    onCancelClicked(void);

    /**
     * @brief onSaveClicked
     */
    void
    onSaveClicked(void);

    /**
     * @brief onDeleteClicked
     */
    void
    onDeleteClicked(void);

    /**
     * @brief When the Content combobox change its value
     * @param text
     */
    void
    onContentComboBoxChanged(const QString &text);

  private:

    /**
     * @brief cleanCurrentContentWidget
     */
    void
    cleanCurrentContentWidgets();

    /**
     * @brief setupContentWidget
     * @param content_widget
     */
    void
    setupContentWidget(ContentWidgetInterface* content_widget);

    /**
     * @brief Configure the ContentComboBox from the current content
     * @param content The current content
     */
    void
    configureContentComboBox(const data::Content::ConstPtr& content);

    /**
     * @brief configureEncryptionCheckbox
     * @param content
     * @param bool
     */
    void
    configureEncryptionCheckbox(const data::Content::ConstPtr& content, bool editable);

    /**
     * @brief Returns the tags for the given content
     * @param content The content to get the tags for
     * @return the list of tags for the given content
     */
    std::vector<data::Tag::ConstPtr>
    contentTags(const data::Content::ConstPtr& content);

    /**
     * @brief setupTagger
     * @param tags
     */
    void
    setupTagger(const std::vector<data::Tag::ConstPtr>& tags);

    /**
     * @brief configureNewContentWidget
     * @param content_widget
     */
    void
    configureNewContentWidget(ContentWidgetInterface* content_widget);


  private:
    Ui::ContentEditorWidget *ui;
    ContentWidgetInterface* content_widget_;
    TaggerWidget* tagger_widget_;
    service::ServiceAPI::Ptr service_api_;
};

} // namespace qt_client

#endif // QT_CLIENT_CONTENT_EDITOR_WIDGET_H_
