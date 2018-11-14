#include "tagmanager.h"

#include <core/debug/Debug.h>

////////////////////////////////////////////////////////////////////////////////
Tag*
TagManager::addInternalTag(const core::UID& id, const Tag& t)
{
    ASSERT(t.id() == id);
    // TODO: tooooo drastic
    ASSERT(getTag(id) == nullptr);
    ASSERT(getTag(t.text()) == nullptr);

    m_TagsMap.insert(std::make_pair(id, t));
    m_TagStringMap.insert(std::make_pair(t.text(), id));

    // we add the suggestion here
    m_prefixTrie.insert(t.text());

    return getTag(id);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
TagManager::TagManager()
{
}

////////////////////////////////////////////////////////////////////////////////
TagManager::~TagManager()
{
}


////////////////////////////////////////////////////////////////////////////////
Tag*
TagManager::createTag(const std::string& text)
{
    const core::UID tagID = core::UID::generateRandom();
    return addInternalTag(tagID, Tag(tagID, text));
}

bool
TagManager::removeTag(const core::UID& id)
{
    const Tag* t = getTag(id);
    if (t == nullptr) {
        return false;
    }
    m_TagStringMap.erase(t->text());
    m_TagsMap.erase(id);
    return true;
}

Tag*
TagManager::addTag(const Tag& t)
{
    if (hasTag(t.id())) {
        debugERROR("We are trying to add a Tag that we already have %s", t.id().toStr().c_str());
        return nullptr;
    }
    return addInternalTag(t.id(), t);
}

void
TagManager::getSuggestedTags(const std::string& prefix, std::vector<const Tag*>& suggestions)
{
    suggestions.clear();
    std::vector<std::string> strSuggestions;
    m_prefixTrie.getSuggestions(prefix, strSuggestions);
    for (auto& strTag : strSuggestions) {
        const Tag* t = getTag(strTag);
        ASSERT_PTR(t);
        suggestions.push_back(t);
    }
}

void
TagManager::getAllTags(std::vector<const Tag*>& Tags)
{
    Tags.clear();
    for (auto it = m_TagsMap.begin(); it != m_TagsMap.end(); ++it) {
        Tags.push_back(&it->second);
    }
}

