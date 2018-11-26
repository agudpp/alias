#ifndef ELEMENT_HANDLER_H
#define ELEMENT_HANDLER_H

#include <deque>

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

    Element::ConstPtr
    selected(void);

  private:

    /**
     * @brief Builds a new element list item using the given element
     * @param element the element
     * @return the widget list item
     */
    ElementWidget*
    itemFromElement(Element::ConstPtr& element);

    /**
     * @brief frees an item
     * @param item the tiem
     */
    void
    freeItem(ElementWidget* item);

    /**
     * @brief Returns the current selected item
     * @return the item
     */
    ElementWidget*
    selectedItem(void);



  private:
    Ui::ElementHandler *ui;
    std::deque<ElementWidget*> elem_queue_;
};

#endif // ELEMENT_HANDLER_H
