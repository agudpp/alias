#ifndef ELEMENT_UI_BASE_H
#define ELEMENT_UI_BASE_H

#include <memory>
#include <QTableWidgetItem>

#include <elements/element.h>

class ElementUIBase {
  public:
    typedef std::shared_ptr<ElementUIBase> Ptr;
  public:
    ElementUIBase();
    virtual ~ElementUIBase() {}

    /**
     * @brief element
     * @return returns the associated element
     */
    inline Element::ConstPtr
    element(void) const
    {
      return element_;
    }

    /**
     * @brief Returns the element type
     * @return
     */
    inline QString
    elementType(void) const
    {
      return element_.get() != nullptr ? element_->elementType().c_str() : "";
    }

    /**
     * @brief setElement
     * @param element the element to be set
     */
    virtual void
    setElement(Element::ConstPtr& element)
    {
      element_ = element;
    }

    /**
     * @brief Should return the representation of this element to be set on the table
     * @return the new created representation of the element
     */
    virtual QTableWidgetItem*
    tableRepresentation(void) const = 0;


  private:
    Element::ConstPtr element_;

};

#endif // ELEMENT_UI_BASE_H
