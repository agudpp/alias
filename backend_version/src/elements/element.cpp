#include "element.h"

////////////////////////////////////////////////////////////////////////////////
void
element::setText(const std::string& t)
{
    // for now just assign
    m_text = t;
}


////////////////////////////////////////////////////////////////////////////////
void
element::addTagID(core::id_t id)
{
    m_tagIDs.insert(id);
}

void
element::removeTagID(core::id_t id)
{
    m_tagIDs.erase(id);
}

bool
element::hasTagID(core::id_t id) const
{
    return m_tagIDs.find(id) != m_tagIDs.end();
}
