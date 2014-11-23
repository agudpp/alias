#ifndef _ELEMENT_H_
#define _ELEMENT_H_


#include <string>

#include <core/types/basic_types.h>


// @brief Root class for the hierarchy of elements we will handle
//

namespace tl {


// Enum for the different types (basic, then we can define more specific like
// PhotoContentElement, ContactContentElement, ConversationContentElement?.. etc)
// but all of them will be of the type Content
//
enum ElementType {
    ET_INVALID = -1,
    ET_TAG = 0,
    ET_LINK,
    ET_CONTENT,

    // THIS SHOULD BE ALWAYS AT THE END
    ET_COUNT,
};


class Element {
public:
    Element() : m_elementID(INVALID_ID), m_elementType(ET_INVALID) {}

    Element(ElementID id, ElementType type) :
        m_elementID(id)
    ,   m_elementType(type)
    {}

    ~Element() {}

    // @brief Return the main information of the element
    //
    inline ElementID
    elementID(void) const;
    inline ElementType
    elementType(void) const;

    // @brief Return the data associated to the element, for all we need
    //        at least a name and desc. Maybe will be empty, maybe not.
    //        This first version will only support this, in the future we
    //        can add more methods and better class structure.
    //
    inline const std::string&
    elementName(void) const;
    inline const std::string&
    elementDesc(void) const;
    inline void
    setElementName(const std::string& name);
    inline void
    setElementDesc(const std::string& desc);


protected:
    std::string m_elementName;
    std::string m_elementDesc;

private:
    ElementID m_elementID;
    ElementType m_elementType;
};










////////////////////////////////////////////////////////////////////////////////
// Inline stuff
//

inline ElementID
Element::elementID(void) const
{
    return m_elementID;
}
inline ElementType
Element::elementType(void) const
{
    return m_elementType;
}


inline const std::string&
Element::elementName(void) const
{
    return m_elementName;
}
inline const std::string&
Element::elementDesc(void) const
{
    return m_elementDesc;
}
inline void
Element::setElementName(const std::string& name)
{
    m_elementName = name;
}

inline void
Element::setElementDesc(const std::string& desc)
{
    m_elementDesc = desc;
}

}


#endif // _ELEMENT_H_
