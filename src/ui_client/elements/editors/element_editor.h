#ifndef ELEMENT_EDITOR_H
#define ELEMENT_EDITOR_H

#include <string>
#include <vector>

#include <QDialog>

#include <service_api/serviceapi.h>
#include <elements/element.h>

#include <ui_client/elements/editors/element_editor_interface.h>
#include <ui_client/tag/tag_handler_widget.h>

namespace Ui {
class ElementEditor;
}

class ElementEditor : public QDialog
{
    Q_OBJECT

  public:
    explicit ElementEditor(QWidget *parent = nullptr, ServiceAPI* service_api = nullptr);
    ~ElementEditor();

    /**
     * @brief Sets the element to be edited
     * @param element the element where we will set the proper informatoin
     * @param with_tags the tags the element already have if any
     */
    void
    editElement(Element::Ptr& element, const std::vector<Tag::ConstPtr>& with_tags);

    /**
     * @brief The edited element
     * @return  the edited element
     */
    Element::Ptr&
    element(void);

    /**
     * @brief The associated assigned tags
     * @return the tags
     */
    const std::vector<std::string>
    tagTexts(void) const;

    /**
     * @brief Executes the editor
     * @return QDialog::exec() result
     */
    int
    executeEditor(void);


  protected slots:

    void
    tagHandlerInputTextChanged(const QString& text);
    void
    tagHandlerTagRemoved(Tag::ConstPtr tag);
    void
    tagHandlerTagSelected(Tag::ConstPtr tag);
    void
    tagHandlerkeyPressed(QKeyEvent* event);

    void
    editionFinished(void);

    void
    editionCancelled(void);


  private:

    /**
     * @brief Performs a search operation gathering all the required information and
     *        the given current text.
     * @param text the text to search for
     */
    void
    performSearch(const QString& text);

    /**
     * @brief builds the ui for the given element
     */
    void
    buildUI(void);

    /**
     * @brief Clear the current ui
     */
    void
    clearUI(void);

  private:
    Ui::ElementEditor *ui;
    Element::Ptr element_;
    std::vector<Tag::ConstPtr> tags_;
    ServiceAPI* service_api_;
    ElementEditorInterface* editor_;
    TagHandlerWidget* tag_handler_;
};

#endif // ELEMENT_EDITOR_H
