#ifndef _LINK_H_
#define _LINK_H_

#include <core/elements/Element.h>

namespace tl {

// @brief This class will represent a link between 2 objects (elements) of the
//        same type except that they are not from the type link itself
//        Since we will manipulate the relations in other classes we will
//        not put the information in here, here is only the Link information
//        without any linking

class LinkElement : public Element {
public:
    LinkElement() : Element(INVALID_ID, ElementType::ET_LINK) {}
    LinkElement(ElementID id) : Element(id, ElementType::ET_LINK) {}

};


}



#endif // _LINK_H_
