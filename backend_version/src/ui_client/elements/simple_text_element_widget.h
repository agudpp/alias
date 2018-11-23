#ifndef SIMPLE_TEXT_ELEMENT_WIDGET_H
#define SIMPLE_TEXT_ELEMENT_WIDGET_H

#include <QWidget>

#include <elements/simple_text_element.h>

namespace Ui {
class SimpleTextElementWidget;
}

class SimpleTextElementWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit SimpleTextElementWidget(QWidget *parent = nullptr);
    ~SimpleTextElementWidget();

    void
    setElement(const SimpleTextElement::ConstPtr& element);

    inline SimpleTextElement::ConstPtr
    element(void) const
    {
      return element_;
    }


  private:
    Ui::SimpleTextElementWidget *ui;
    SimpleTextElement::ConstPtr element_;
};

#endif // SIMPLE_TEXT_ELEMENT_WIDGET_H
