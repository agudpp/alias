#ifndef DATA_DATA_MAPPER_H_
#define DATA_DATA_MAPPER_H_

#include <unordered_map>
#include <set>
#include <vector>
#include <memory>

#include <toolbox/types/trie.h>

#include <data/tag.h>
#include <data/content.h>


namespace data {

class DataMapper
{
  public:

    using Ptr = std::shared_ptr<DataMapper>;

  public:
    DataMapper() = default;
    ~DataMapper() = default;

    DataMapper(const DataMapper&) = delete;
    DataMapper& operator=(const DataMapper&) = delete;
    DataMapper(DataMapper&&) = delete;
    DataMapper& operator=(DataMapper&&) = delete;

    // TODO: add doxygen

    bool
    addTag(Tag::Ptr tag);
    bool
    removeTag(Tag::Ptr tag);
    bool
    hasTag(const Tag::Ptr& tag) const;
    bool
    hasTag(const std::string& tag_name) const;
    bool
    hasTag(const toolbox::UID& tag_id) const;
    Tag::Ptr
    tagFromName(const std::string& name) const;
    Tag::Ptr
    tagFromID(const toolbox::UID& id) const;
    std::set<Content::Ptr>
    contentsForTag(const toolbox::UID& tag_id) const;
    std::set<Tag::Ptr>
    suggestedTags(const std::string& prefix) const;

    bool
    addContent(Content::Ptr content);
    bool
    removeContent(Content::Ptr content);
    bool
    hasContent(const Content::Ptr& content) const;
    bool
    hasContent(const toolbox::UID& id) const;
    Content::Ptr
    contentFromID(const toolbox::UID& id) const;


  private:

    struct TagBackRefData {
      Tag::Ptr tag;
      std::set<toolbox::UID> contents;
    };

    using ContentMap = std::unordered_map<toolbox::UID, Content::Ptr>;
    using TagMap = std::unordered_map<toolbox::UID, TagBackRefData>;
    using TagNameMap = std::unordered_map<std::string, toolbox::UID>;


  private:

    bool
    removeTagForContents(const Tag::Ptr& tag, const std::set<toolbox::UID>& content_ids);

    bool
    removeContentForTags(const Content::Ptr& content, const std::set<toolbox::UID>& tag_ids);

    void
    addTagContentBackrefs(const Content::Ptr& content);

  private:
    ContentMap content_map_;
    TagMap tag_map_;
    TagNameMap tag_name_map_;
    toolbox::trie tag_prefixes_;

};

} // namespace data

#endif // DATA_DATA_MAPPER_H_
