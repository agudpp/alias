#ifndef ELEMENT_UI_BUILDER_H
#define ELEMENT_UI_BUILDER_H

#include <elements/element.h>

#include <ui_client/elements/types/element_ui_base.h>


class ElementUIBuilder
{
  public:

    /**
     * @brief build
     * @param element
     * @return
     */
    static ElementUIBase::Ptr
    build(Element::ConstPtr& element);

};

#endif // ELEMENT_UI_BUILDER_H
