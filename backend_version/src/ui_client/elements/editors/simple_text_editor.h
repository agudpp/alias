#ifndef simple_text_editor_H
#define simple_text_editor_H

#include <QWidget>

#include <elements/element.h>

#include <ui_client/elements/editors/element_editor_interface.h>


namespace Ui {
class SimpleTextEditor;
}

class SimpleTextEditor : public ElementEditorInterface
{
    Q_OBJECT

  public:
    explicit SimpleTextEditor(QWidget *parent = nullptr);
    ~SimpleTextEditor();

    virtual void
    saveEdition(void);

    virtual void
    prepareEdition(void);

  signals:

    /**
     * @brief when the edition is finished successfully
     */
    void editionFinished(void);

    /**
     * @brief Edition was cancelled
     */
    void editionCancelled(void);


  private:
    Ui::SimpleTextEditor *ui;
};

#endif // simple_text_editor_H
