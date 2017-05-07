#ifndef ELEMENTMANAGER_H
#define ELEMENTMANAGER_H

#include <unordered_map>
#include <string>

#include <core/types/id_type.h>

#include "element.h"


class ElementManager
{
public:
    ElementManager();
    ~ElementManager();

    bool
    createElement(const std::string& text);
    bool
    removeElement(core::id_t id);
    bool
    addElement(const element& element);

    inline bool
    hasElement(core::id_t id) const;
    inline const element*
    getElement(core::id_t id) const;
    inline element*
    getElement(core::id_t id);

private:
    typedef std::unordered_map<core::id_t, element> ElementHash;

private:
    ElementHash m_elementsMap;
    core::int32_t m_maxID;

};




////////////////////////////////////////////////////////////////////////////////

inline bool
ElementManager::hasElement(core::id_t id) const
{
    return m_elementsMap.find(id) != m_elementsMap.end();
}
inline const element*
ElementManager::getElement(core::id_t id) const
{
    auto it = m_elementsMap.find(id);
    return (it == m_elementsMap.end()) ? 0 : &it->second;
}
inline element*
ElementManager::getElement(core::id_t id)
{
    auto it = m_elementsMap.find(id);
    return (it == m_elementsMap.end()) ? 0 : &it->second;
}


#endif // ELEMENTMANAGER_H
