#ifndef DATA_DATA_HANDLER_H_
#define DATA_DATA_HANDLER_H_

#include <unordered_map>
#include <set>

#include <data/tag.h>
#include <data/relation.h>
#include <data/content.h>


namespace data {

class DataHandler
{
  public:
    DataHandler() = default;
    ~DataHandler() = default;

    DataHandler(const DataHandler&) = delete;
    DataHandler& operator=(const DataHandler&) = delete;
    DataHandler(DataHandler&&) = delete;
    DataHandler& operator=(DataHandler&&) = delete;

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

    bool
    relate(const Content::Ptr& content, const std::set<Tag::Ptr>& tags);
    bool
    relate(const Relation& relation);



  private:
    struct ContentData {
      Content::Ptr content;
      std::set<toolbox::UID> tags;
    };

    struct TagData {
      Tag::Ptr tag;
      std::set<toolbox::UID> contents;
    };

    using ContentMap = std::unordered_map<toolbox::UID, ContentData>;
    using TagMap = std::unordered_map<toolbox::UID, TagData>;
    using TagNameMap = std::unordered_map<std::string, toolbox::UID>;


  private:

    bool
    removeTagForContents(const Tag::Ptr& tag, const std::set<toolbox::UID>& content_ids);

    bool
    removeContentForTags(const Content::Ptr& content, const std::set<toolbox::UID>& tag_ids);

  private:
    ContentMap content_map_;
    TagMap tag_map_;
    TagNameMap tag_name_map_;

};

} // namespace data

#endif // DATA_DATA_HANDLER_H_
