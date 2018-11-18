#ifndef TAGMANAGER_H
#define TAGMANAGER_H

#include <unordered_map>
#include <string>
#include <vector>

#include <core/types/trie.h>
#include <core/types/id_type.h>

#include <tags/tag.h>



class TagManager
{
public:
    TagManager();
    ~TagManager();

    Tag::Ptr
    createTag(const std::string& text);
    bool
    removeTag(const core::UID& id);
    Tag::Ptr
    addTag(const Tag& t);
    Tag::Ptr
    getOrCreate(const std::string& text);

    inline bool
    hasTag(const core::UID& id) const;
    inline Tag::ConstPtr
    getTag(const core::UID& id) const;
    inline Tag::Ptr
    getTag(const core::UID& id);

    inline Tag::ConstPtr
    getTag(const std::string& t) const;
    inline Tag::Ptr
    getTag(const std::string& t);
    inline std::vector<Tag::ConstPtr>
    getTags(const std::set<core::UID>& ids) const;
    inline std::vector<Tag::Ptr>
    getTags(const std::set<core::UID>& ids);


    ///
    /// \brief getSuggestedTags
    /// \param prefix
    /// \param suggestions
    ///
    void
    getSuggestedTags(const std::string& prefix, std::vector<Tag::ConstPtr>& suggestions);

    ///
    /// \brief getAllTags will return the list of all Tags, this should be an iterator
    /// \param Tags
    ///
    void
    getAllTags(std::vector<Tag::ConstPtr>& Tags);

private:
    typedef std::unordered_map<core::UID, Tag::Ptr> TagHash;
    typedef std::unordered_map<std::string, core::UID> TagStringHash;


private:

    Tag::Ptr
    addInternalTag(const core::UID& id, const Tag& t);

private:
    TagHash tags_map_;
    TagStringHash tag_string_map_;
    trie prefix_trie_;
};


////////////////////////////////////////////////////////////////////////////////

inline bool
TagManager::hasTag(const core::UID& id) const
{
    return tags_map_.find(id) != tags_map_.end();
}
inline Tag::ConstPtr
TagManager::getTag(const core::UID& id) const
{
    auto it = tags_map_.find(id);
    return (it == tags_map_.end()) ? Tag::ConstPtr() : it->second;
}
inline Tag::Ptr
TagManager::getTag(const core::UID& id)
{
    auto it = tags_map_.find(id);
    return (it == tags_map_.end()) ? Tag::Ptr() : it->second;
}

inline Tag::ConstPtr
TagManager::getTag(const std::string& t) const
{
    auto it = tag_string_map_.find(t);
    return (it == tag_string_map_.end()) ? Tag::ConstPtr() : getTag(it->second);
}
inline Tag::Ptr
TagManager::getTag(const std::string& t)
{
    auto it = tag_string_map_.find(t);
    return (it == tag_string_map_.end()) ? Tag::Ptr() : getTag(it->second);
}

inline std::vector<Tag::ConstPtr>
TagManager::getTags(const std::set<core::UID>& ids) const
{
  std::vector<Tag::ConstPtr> result;
  for (const core::UID& id : ids) {
    Tag::ConstPtr tag = getTag(id);
    if (tag.get() != nullptr) {
      result.push_back(tag);
    }
  }
  return result;
}

inline std::vector<Tag::Ptr>
TagManager::getTags(const std::set<core::UID>& ids)
{
  std::vector<Tag::Ptr> result;
  for (const core::UID& id : ids) {
    Tag::Ptr tag = getTag(id);
    if (tag.get() != nullptr) {
      result.push_back(tag);
    }
  }
  return result;
}




#endif // TAGMANAGER_H
