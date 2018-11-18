#include <tags/tagmanager.h>

#include <core/debug/Debug.h>

////////////////////////////////////////////////////////////////////////////////
Tag::Ptr
TagManager::addInternalTag(const core::UID& id, const Tag& t)
{
    ASSERT(t.id() == id);
    // TODO: tooooo drastic
    ASSERT(getTag(id) == nullptr);
    ASSERT(getTag(t.text()) == nullptr);

    tags_map_.insert(std::make_pair(id, Tag::Ptr(new Tag(t))));
    tag_string_map_.insert(std::make_pair(t.text(), id));

    // we add the suggestion here
    prefix_trie_.insert(t.text());

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
Tag::Ptr
TagManager::createTag(const std::string& text)
{
    const core::UID tagID = core::UID::generateRandom();
    return addInternalTag(tagID, Tag(tagID, text));
}

bool
TagManager::removeTag(const core::UID& id)
{
    Tag::Ptr t = getTag(id);
    if (t.get() == nullptr) {
        return false;
    }
    tag_string_map_.erase(t->text());
    tags_map_.erase(id);
    return true;
}

Tag::Ptr
TagManager::addTag(const Tag& t)
{
    if (hasTag(t.id())) {
        debugERROR("We are trying to add a Tag that we already have %s", t.id().toStr().c_str());
        return nullptr;
    }
    return addInternalTag(t.id(), t);
}

Tag::Ptr
TagManager::getOrCreate(const std::string& text)
{
  Tag::Ptr result = getTag(text);
  return result.get() == nullptr ? createTag(text) : result;
}

void
TagManager::getSuggestedTags(const std::string& prefix, std::vector<Tag::ConstPtr>& suggestions)
{
    suggestions.clear();
    std::vector<std::string> strSuggestions;
    prefix_trie_.getSuggestions(prefix, strSuggestions);
    for (auto& strTag : strSuggestions) {
        Tag::ConstPtr t = getTag(strTag);
        ASSERT_PTR(t.get());
        suggestions.push_back(t);
    }
}

void
TagManager::getAllTags(std::vector<Tag::ConstPtr>& tags)
{
    tags.clear();
    for (auto it = tags_map_.begin(); it != tags_map_.end(); ++it) {
        tags.push_back(it->second);
    }
}

