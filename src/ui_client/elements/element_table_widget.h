#ifndef ELEMENT_TABLE_WIDGET_H
#define ELEMENT_TABLE_WIDGET_H


#include <QTableWidget>

#include <ui_client/elements/types/element_ui_base.h>


class ElementTableWidget
{
  public:
    ElementTableWidget();
    ~ElementTableWidget();

    /**
     * @brief getRepresentation
     * @return
     */
    inline QTableWidget*
    getRepresentation(void)
    {
      return table_;
    }

    // add element
    void
    addElement(const ElementUIBase* element);

    // remove element

    // clear
    void
    clear(void);

    // select next
    void
    selectNext(void);

    // select previous
    void
    selectPrevious(void);

    // unselect current
    void
    unselectCurrent(void);

    bool
    hasSelected(void) const;

    // get selected
    const ElementUIBase*
    selected(void) const;

  private:

    /**
     * @brief configureTable internally
     */
    void
    configureTable(void);

    /**
     * @brief getTypeItem
     * @param element
     * @return
     */
    QTableWidgetItem*
    getTypeItem(const ElementUIBase* element) const;

  private:
    QTableWidget* table_;

};

#endif // ELEMENT_TABLE_WIDGET_H
