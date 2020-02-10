#ifndef ELEMENT_HANDLER_H
#define ELEMENT_HANDLER_H

#include <QWidget>
#include <QListWidget>

#include <core/types/auto_free_queue.h>

#include <elements/element.h>

#include <ui_client/elements/types/element_ui_base.h>
#include <ui_client/elements/element_table_widget.h>

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



  private:
    Ui::ElementHandler *ui;
    std::vector<ElementUIBase::Ptr> elements_;
    ElementTableWidget table_;
};

#endif // ELEMENT_HANDLER_H
