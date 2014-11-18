#ifndef ELEMENTHOLDER_H
#define ELEMENTHOLDER_H

#include <map>

#include <core/types/basic_types.h>
#include <core/debug/DebugUtil.h>

namespace tl {

// @brief This class will be used to add / remove / query elements in a faster
//        way
//        The type of the template should at least respect the interface
//        of a Element (getElementID).
//

template<typename Type>
class ElementHolder {
public:
    ElementHolder(){};
    ~ElementHolder(){}

    ////////////////////////////////////////////////////////////////////////////
    // API
    //

    // @brief Add a new element to the holder, this class will not will
    //        handle the memory for the element.
    // @param element       The element to be added.
    // @return true on success | false otherwise
    //
    inline bool
    addElement(const Type* element);

    // @brief Return an element if exists in the holder from and ID
    // @param id        The id of the element we want to check if exists or not
    // @return the associated element if exists | false otherwise
    //
    inline const Type*
    getElement(ElementID id) const;

    // @brief Remove an element from the holder.
    // @param id        The id of the element we want to remove
    // @return true on success | false otherwise
    //
    inline bool
    removeElement(ElementID id);


private:
    // TODO: for now we will use a hash table, we can try to change it in the
    // future for a better structure like vector and assign ids and save the
    // unusued ID's in a list for later use (reuse of ids so we have a maximum
    // number of element with the best vector size).
    //
    typedef std::map<ElementID, const Type*> ElementHash;

    ElementHash m_hashMap;
};







////////////////////////////////////////////////////////////////////////////////
// Inline
//

template<typename Type>
inline bool
ElementHolder<Type>::addElement(const Type* element)
{
    ASSERT(element != 0);
    ASSERT(getElement(element->elementID()) == 0);
    m_hashMap[element->elementID()] = element;
}

template<typename Type>
inline const Type*
ElementHolder<Type>::getElement(ElementID id) const
{
    auto it = m_hashMap.find(id);
    if (it == m_hashMap.end()) {
        return 0;
    }
    return it->second;
}

template<typename Type>
inline bool
ElementHolder<Type>::removeElement(ElementID id)
{
    auto it = m_hashMap.find(id);
    if (it == m_hashMap.end()) {
        return false;
    }

    m_hashMap.erase(it);
    return true;
}

}

#endif // ELEMENTHOLDER_H
