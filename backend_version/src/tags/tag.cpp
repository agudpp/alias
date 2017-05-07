#include "tag.h"

////////////////////////////////////////////////////////////////////////////////
void
tag::setText(const std::string& t)
{
    // for now just assign
    m_text = t;
}


////////////////////////////////////////////////////////////////////////////////
void
tag::addElementID(core::id_t elemID)
{
    m_elementIDs.insert(elemID);
}

void
tag::removeElementID(core::id_t elemID)
{
    m_elementIDs.erase(elemID);
}

bool
tag::hasElementID(core::id_t elemID) const
{
    return m_elementIDs.find(elemID) != m_elementIDs.end();
}
