#ifndef ELEMENT_WIDGET_H
#define ELEMENT_WIDGET_H

#include <QListWidgetItem>

#include <elements/element.h>

// TODO: rename to ElementWidgetItem?
class ElementWidget : public QListWidgetItem
{
  public:
    explicit ElementWidget(QListWidget *parent = nullptr);
    ~ElementWidget();

    inline Element::ConstPtr
    element(void) const
    {
      return element_;
    }

    void
    setElement(Element::ConstPtr& element);

  private:
    Element::ConstPtr element_;
};

#endif // ELEMENT_WIDGET_H
