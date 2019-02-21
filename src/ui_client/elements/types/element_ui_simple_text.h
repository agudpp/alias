#ifndef ELEMENT_UI_SIMPLE_TEXT_H
#define ELEMENT_UI_SIMPLE_TEXT_H

#include <ui_client/elements/types/element_ui_base.h>


class ElementUISimpleText : public ElementUIBase {
  public:
    ElementUISimpleText(void);
    virtual ~ElementUISimpleText(void) {}


    virtual QTableWidgetItem*
    tableRepresentation(void) const override;

};

#endif // ELEMENT_UI_SIMPLE_TEXT_H
