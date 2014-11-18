#ifndef _TAG_H_
#define _TAG_H_

#include <core/elements/Element.h>

namespace tl {

class TagElement : public Element {
public:
    TagElement() : Element(INVALID_ID, ElementType::ET_TAG) {}
    TagElement(ElementID id) : Element(id, ElementType::ET_TAG) {}


    ////////////////////////////////////////////////////////////////////////////
    // TAG methods
    //

    // @brief Set / get the color
    //
    inline ColorType
    color(void) const;
    inline void
    setColor(ColorType c);

protected:
    ColorType m_color;
};




////////////////////////////////////////////////////////////////////////////////
// Inline stuff
//

inline ColorType
TagElement::color(void) const
{
    return m_color;
}
inline void
TagElement::setColor(ColorType c)
{
    m_color = c;
}

}



#endif // _TAG_H_
