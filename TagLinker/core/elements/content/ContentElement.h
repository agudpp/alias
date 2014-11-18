#ifndef _CONTENT_H_
#define _CONTENT_H_

#include <core/elements/Element.h>

namespace tl {

class ContentElement : public Element {
public:
    ContentElement() : Element(INVALID_ID, ElementType::ET_CONTENT) {}
    ContentElement(ElementID id) : Element(id, ElementType::ET_CONTENT) {}

};



#endif // _CONTENT_H_
