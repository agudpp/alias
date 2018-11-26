#ifndef ELEMENT_EDITOR_INTERFACE_H
#define ELEMENT_EDITOR_INTERFACE_H

#include <QWidget>

#include <elements/element.h>


class ElementEditorInterface : public QWidget
{
    Q_OBJECT

  public:
    explicit ElementEditorInterface(QWidget *parent = nullptr) : QWidget(parent) {}
    virtual ~ElementEditorInterface() {}


    virtual void
    saveEdition(void) = 0;

    virtual void
    prepareEdition(void) = 0;

    inline void
    setElement(Element::Ptr element)
    {
      element_ = element;
    }

    inline Element::Ptr
    element(void)
    {
      return element_;
    }

  signals:

    /**
     * @brief when the edition is finished successfully
     */
    void editionFinished(void);

    /**
     * @brief Edition was cancelled
     */
    void editionCancelled(void);

  protected:
    Element::Ptr element_;
};

#endif // ELEMENT_EDITOR_INTERFACE_H
