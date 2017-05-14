#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <unordered_map>
#include <string>

#include <core/types/trie.h>
#include <core/types/id_type.h>

#include "tag.h"



class TagManager
{
public:
    TagManager();
    ~TagManager();

    tag*
    createTag(const std::string& text);
    bool
    removeTag(core::id_t id);
    tag *
    addTag(const tag& t);

    inline bool
    hasTag(core::id_t id) const;
    inline const tag*
    getTag(core::id_t id) const;
    inline tag*
    getTag(core::id_t id);

    inline const tag*
    getTag(const std::string& t) const;
    inline tag*
    getTag(const std::string& t);

    ///
    /// \brief getSuggestedTags
    /// \param prefix
    /// \param suggestions
    ///
    void
    getSuggestedTags(const std::string& prefix, std::vector<const tag*>& suggestions);


private:
    typedef std::unordered_map<core::id_t, tag> TagHash;
    typedef std::unordered_map<std::string, core::id_t> TagStringHash;


private:

    tag*
    addInternalTag(core::id_t id, const tag& t);

private:
    TagHash m_tagsMap;
    TagStringHash m_tagStringMap;
    core::int32_t m_maxID;
    trie m_prefixTrie;
};


////////////////////////////////////////////////////////////////////////////////

inline bool
TagManager::hasTag(core::id_t id) const
{
    return m_tagsMap.find(id) != m_tagsMap.end();
}
inline const tag*
TagManager::getTag(core::id_t id) const
{
    auto it = m_tagsMap.find(id);
    return (it == m_tagsMap.end()) ? 0 : &it->second;
}
inline tag*
TagManager::getTag(core::id_t id)
{
    auto it = m_tagsMap.find(id);
    return (it == m_tagsMap.end()) ? 0 : &it->second;
}

inline const tag*
TagManager::getTag(const std::string& t) const
{
    auto it = m_tagStringMap.find(t);
    return (it == m_tagStringMap.end()) ? 0 : getTag(it->second);
}
inline tag*
TagManager::getTag(const std::string& t)
{
    auto it = m_tagStringMap.find(t);
    return (it == m_tagStringMap.end()) ? 0 : getTag(it->second);
}




#endif // TAGMANAGER_H
