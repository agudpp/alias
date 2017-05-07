#include "elementmanager.h"

#include <core/debug/Debug.h>


////////////////////////////////////////////////////////////////////////////////
ElementManager::ElementManager() :
    m_maxID(0)
{
}

////////////////////////////////////////////////////////////////////////////////
ElementManager::~ElementManager()
{
}


////////////////////////////////////////////////////////////////////////////////
bool
ElementManager::createElement(const std::string& text)
{
    const core::id_t elemID = core::id_t(m_maxID++);
    auto ret = m_elementsMap.insert(std::make_pair(elemID, element(elemID, text)));
    return ret.second;
}

bool
ElementManager::removeElement(core::id_t id)
{
    return m_elementsMap.erase(id) > 0;
}

bool
ElementManager::addElement(const element& e)
{
    if (hasElement(e.id())) {
        debugERROR("We are trying to add a element that we already have %d", e.id());
        return false;
    }
    m_maxID = std::max(m_maxID, e.id());
    m_elementsMap.insert(std::make_pair(e.id(), e));
    return true;
}
