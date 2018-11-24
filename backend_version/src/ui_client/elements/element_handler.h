#ifndef ELEMENT_HANDLER_H
#define ELEMENT_HANDLER_H

#include <QWidget>
#include <QListWidget>

#include <elements/element.h>

#include <ui_client/elements/element_widget.h>

namespace Ui {
class ElementHandler;
}

class ElementHandler : public QWidget
{
    Q_OBJECT

  public:
    explicit ElementHandler(QWidget *parent = nullptr);
    ~ElementHandler();


    void
    setElements(const std::vector<Element::ConstPtr>& elements);

    void
    clear(void);

    bool
    selectNext(void);

    bool
    selectPrev(void);

    void
    unselectCurrent(void);

    bool
    hasSelected(void) const;

    bool
    executeSelected(void);

    bool
    editSelected(void);

  private:

    /**
     * @brief Builds a new element list item using the given element
     * @param element the element
     * @return the widget list item
     */
    ElementWidget*
    itemFromElement(Element::ConstPtr& element);


  private:
    Ui::ElementHandler *ui;
};

#endif // ELEMENT_HANDLER_H
