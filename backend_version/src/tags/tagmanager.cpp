#include "tagmanager.h"

#include <core/debug/Debug.h>

////////////////////////////////////////////////////////////////////////////////
bool
TagManager::addInternalTag(core::id_t id, const tag& t)
{
    ASSERT(t.id() == id);
    // TODO: tooooo drastic
    ASSERT(getTag(id) == 0);
    ASSERT(getTag(t.text()) == 0);

    auto ret = m_tagsMap.insert(std::make_pair(id, t));
    m_tagStringMap.insert(std::make_pair(t.text(), id));

    // we add the suggestion here
    m_prefixTrie.insert(t.text());

    return ret.second;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
TagManager::TagManager() :
    m_maxID(0)
{
}

////////////////////////////////////////////////////////////////////////////////
TagManager::~TagManager()
{
}


////////////////////////////////////////////////////////////////////////////////
bool
TagManager::createTag(const std::string& text)
{
    const core::id_t tagID = core::id_t(m_maxID++);
    return addInternalTag(tagID, tag(tagID, text));
}

bool
TagManager::removeTag(core::id_t id)
{
    const tag* t = getTag(id);
    if (t == 0) {
        return false;
    }
    m_tagStringMap.erase(t->text());
    m_tagsMap.erase(id);
    return true;
}

bool
TagManager::addTag(const tag& t)
{
    if (hasTag(t.id())) {
        debugERROR("We are trying to add a tag that we already have %d", t.id());
        return false;
    }
    m_maxID = std::max(m_maxID, t.id()) + 1;
    return addInternalTag(t.id(), t);
}

void
TagManager::getSuggestedTags(const std::string& prefix, std::vector<const tag*>& suggestions)
{
    suggestions.clear();
    std::vector<std::string> strSuggestions;
    m_prefixTrie.getSuggestions(prefix, strSuggestions);
    for (auto& strTag : strSuggestions) {
        const tag* t = getTag(strTag);
        ASSERT_PTR(t);
        suggestions.push_back(t);
    }
}

