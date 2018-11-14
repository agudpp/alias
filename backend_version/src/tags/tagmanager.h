#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>

#include <core/types/trie.h>
#include <core/types/id_type.h>

#include "tag.h"



class TagManager
{
public:
    TagManager();
    ~TagManager();

    Tag*
    createTag(const std::string& text);
    bool
    removeTag(const core::UID& id);
    Tag *
    addTag(const Tag& t);

    inline bool
    hasTag(const core::UID& id) const;
    inline const Tag*
    getTag(const core::UID& id) const;
    inline Tag*
    getTag(const core::UID& id);

    inline const Tag*
    getTag(const std::string& t) const;
    inline Tag*
    getTag(const std::string& t);

    ///
    /// \brief getSuggestedTags
    /// \param prefix
    /// \param suggestions
    ///
    void
    getSuggestedTags(const std::string& prefix, std::vector<const Tag*>& suggestions);

    ///
    /// \brief getAllTags will return the list of all Tags, this should be an iterator
    /// \param Tags
    ///
    void
    getAllTags(std::vector<const Tag*>& Tags);

private:
    typedef std::unordered_map<core::UID, Tag> TagHash;
    typedef std::unordered_map<std::string, core::UID> TagStringHash;


private:

    Tag*
    addInternalTag(const core::UID& id, const Tag& t);

private:
    TagHash m_TagsMap;
    TagStringHash m_TagStringMap;
    trie m_prefixTrie;
};


////////////////////////////////////////////////////////////////////////////////

inline bool
TagManager::hasTag(const core::UID& id) const
{
    return m_TagsMap.find(id) != m_TagsMap.end();
}
inline const Tag*
TagManager::getTag(const core::UID& id) const
{
    auto it = m_TagsMap.find(id);
    return (it == m_TagsMap.end()) ? nullptr : &it->second;
}
inline Tag*
TagManager::getTag(const core::UID& id)
{
    auto it = m_TagsMap.find(id);
    return (it == m_TagsMap.end()) ? nullptr : &it->second;
}

inline const Tag*
TagManager::getTag(const std::string& t) const
{
    auto it = m_TagStringMap.find(t);
    return (it == m_TagStringMap.end()) ? nullptr : getTag(it->second);
}
inline Tag*
TagManager::getTag(const std::string& t)
{
    auto it = m_TagStringMap.find(t);
    return (it == m_TagStringMap.end()) ? nullptr : getTag(it->second);
}




#endif // TAGMANAGER_H
