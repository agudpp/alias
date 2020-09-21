#include <data/data_mapper.h>

#include <toolbox/debug/debug.h>


namespace data {


bool
DataMapper::removeTagForContents(const Tag::Ptr& tag, const std::set<toolbox::UID>& content_ids)
{
  const toolbox::UID& tag_id = tag->id();
  for (const toolbox::UID& content_id : content_ids) {
    auto itr = content_map_.find(content_id);
    if (itr != content_map_.end()) {
      itr->second->removeTag(tag_id);
    }
  }
  return true;
}

bool
DataMapper::removeContentForTags(const Content::Ptr& content, const std::set<toolbox::UID>& tag_ids)
{
  const toolbox::UID& content_id = content->id();
  for (const toolbox::UID& tag_id : tag_ids) {
    auto itr = tag_map_.find(tag_id);
    if (itr != tag_map_.end()) {
      itr->second.contents.erase(content_id);
    }
  }
  return true;
}

void
DataMapper::addTagContentBackrefs(const Content::Ptr& content)
{
  for (auto& tag_id : content->tagIDs()) {
    ASSERT(hasTag(tag_id));
    auto itr = tag_map_.find(tag_id);
    itr->second.contents.insert(content->id());
  }
}

bool
DataMapper::addTag(Tag::Ptr tag)
{
  if (tag.get() == nullptr) {
    LOG_INFO("tag is null");
    return false;
  }

  TagBackRefData& tag_data = tag_map_[tag->id()];
  tag_data.tag = tag;
  tag_name_map_[tag->name()] = tag->id();
  tag_prefixes_.insert(tag->name());

  return true;
}

bool
DataMapper::removeTag(Tag::Ptr tag)
{
  if (tag.get() == nullptr) {
    LOG_INFO("tag is null");
    return false;
  }
  auto itr = tag_map_.find(tag->id());
  if (itr == tag_map_.end()) {
    return false;
  }

  TagBackRefData& tag_data = itr->second;

  if (!removeTagForContents(tag_data.tag, tag_data.contents)) {
    return false;
  }

  tag_map_.erase(itr);
  tag_name_map_.erase(tag->name());

  return true;
}

bool
DataMapper::hasTag(const Tag::ConstPtr& tag) const
{
  return tag.get() != nullptr && tag_map_.find(tag->id()) != tag_map_.end();
}

bool
DataMapper::hasTag(const Tag::Ptr& tag) const
{
  return tag.get() != nullptr && tag_map_.find(tag->id()) != tag_map_.end();
}

bool
DataMapper::hasTag(const std::string& tag_name) const
{
  return tag_name_map_.find(tag_name) != tag_name_map_.end();
}

bool
DataMapper::hasTag(const toolbox::UID& tag_id) const
{
  return tag_map_.find(tag_id) != tag_map_.end();
}

Tag::Ptr
DataMapper::tagFromName(const std::string& name) const
{
  auto itr = tag_name_map_.find(name);
  if (itr == tag_name_map_.end()) {
    return Tag::Ptr();
  }

  auto tag_itr = tag_map_.find(itr->second);

  return tag_itr == tag_map_.end() ? Tag::Ptr() : tag_itr->second.tag;
}

Tag::Ptr
DataMapper::tagFromID(const toolbox::UID& id) const
{
  auto itr = tag_map_.find(id);
  return itr == tag_map_.end() ? Tag::Ptr() : itr->second.tag;
}

std::set<Tag::Ptr>
DataMapper::tagsFromIDs(const std::set<toolbox::UID>& ids) const
{
  std::set<Tag::Ptr> tags;
  for (const auto& id : ids) {
    Tag::Ptr tag_ptr = tagFromID(id);
    if (tag_ptr.get() != nullptr) {
      tags.insert(tag_ptr);
    }
  }
  return tags;
}


std::set<Content::Ptr>
DataMapper::contentsForTag(const toolbox::UID& tag_id) const
{
  if (!hasTag(tag_id)) {
    return std::set<Content::Ptr>();
  }
  auto itr = tag_map_.find(tag_id);
  std::set<Content::Ptr> result;
  for (auto& content_id : itr->second.contents) {
    result.insert(contentFromID(content_id));
  }

  return result;
}

std::set<Tag::Ptr>
DataMapper::suggestedTags(const std::string& prefix) const
{
  std::vector<std::string> suggestions;
  tag_prefixes_.getSuggestions(prefix, suggestions);

  std::set<Tag::Ptr> result;
  for (const std::string& suggestion : suggestions) {
    Tag::Ptr tag = tagFromName(suggestion);
    if (tag.get() != nullptr) {
      result.insert(tag);
    }
  }

  return result;
}

bool
DataMapper::addContent(Content::Ptr content)
{
  if (content.get() == nullptr) {
    LOG_INFO("Content is null, we will not add this");
    return false;
  }

  content_map_[content->id()] = content;
  addTagContentBackrefs(content);

  return true;
}

bool
DataMapper::removeContent(Content::Ptr content)
{
  if (content.get() == nullptr) {
    LOG_INFO("Content is null");
    return false;
  }
  auto itr = content_map_.find(content->id());
  if (itr == content_map_.end()) {
    return false;
  }

  removeContentForTags(itr->second, itr->second->tagIDs());
  content_map_.erase(itr);

  return true;
}

bool
DataMapper::hasContent(const Content::Ptr& content) const
{
  return content.get() != nullptr && content_map_.find(content->id()) != content_map_.end();
}

bool
DataMapper::hasContent(const toolbox::UID& id) const
{
  return content_map_.find(id) != content_map_.end();
}

Content::Ptr
DataMapper::contentFromID(const toolbox::UID& id) const
{
  auto itr = content_map_.find(id);
  return itr == content_map_.end() ? Content::Ptr() : itr->second;
}

}
