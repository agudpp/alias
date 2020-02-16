#include <data/data_handler.h>

#include <toolbox/debug/debug.h>


namespace data {


bool
DataHandler::removeTagForContents(const Tag::Ptr& tag, const std::set<toolbox::UID>& content_ids)
{
  const toolbox::UID& tag_id = tag->id();
  for (const toolbox::UID& content_id : content_ids) {
    auto itr = content_map_.find(content_id);
    if (itr != content_map_.end()) {
      itr->second.tags.erase(tag_id);
    }
  }
  return true;
}

bool
DataHandler::removeContentForTags(const Content::Ptr& content, const std::set<toolbox::UID>& tag_ids)
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

bool
DataHandler::addTag(Tag::Ptr tag)
{
  if (tag.get() == nullptr) {
    LOG_INFO("tag is null");
    return false;
  }

  TagData& tag_data = tag_map_[tag->id()];
  tag_data.tag = tag;
  tag_name_map_[tag->name()] = tag->id();

  return true;
}

bool
DataHandler::removeTag(Tag::Ptr tag)
{
  if (tag.get() == nullptr) {
    LOG_INFO("tag is null");
    return false;
  }
  auto itr = tag_map_.find(tag->id());
  if (itr == tag_map_.end()) {
    return false;
  }

  TagData& tag_data = itr->second;

  if (!removeTagForContents(tag_data.tag, tag_data.contents)) {
    return false;
  }

  tag_map_.erase(itr);
  tag_name_map_.erase(tag->name());

  return true;
}

bool
DataHandler::hasTag(const Tag::Ptr& tag) const
{
  return tag.get() != nullptr && tag_map_.find(tag->id()) != tag_map_.end();
}

bool
DataHandler::hasTag(const std::string& tag_name) const
{
  return tag_name_map_.find(tag_name) != tag_name_map_.end();
}

bool
DataHandler::hasTag(const toolbox::UID& tag_id) const
{
  return tag_map_.find(tag_id) != tag_map_.end();
}

Tag::Ptr
DataHandler::tagFromName(const std::string& name) const
{
  auto itr = tag_name_map_.find(name);
  if (itr == tag_name_map_.end()) {
    return Tag::Ptr();
  }

  auto tag_itr = tag_map_.find(itr->second);

  return tag_itr == tag_map_.end() ? Tag::Ptr() : tag_itr->second.tag;
}

Tag::Ptr
DataHandler::tagFromID(const toolbox::UID& id) const
{
  auto itr = tag_map_.find(id);
  return itr == tag_map_.end() ? Tag::Ptr() : itr->second.tag;
}

bool
DataHandler::addContent(Content::Ptr content)
{
  if (content.get() == nullptr) {
    LOG_INFO("Content is null, we will not add this");
    return false;
  }

  ContentData& content_data = content_map_[content->id()];
  content_data.content = content;

  return true;
}

bool
DataHandler::removeContent(Content::Ptr content)
{
  if (content.get() == nullptr) {
    LOG_INFO("Content is null");
    return false;
  }
  auto itr = content_map_.find(content->id());
  if (itr == content_map_.end()) {
    return false;
  }

  removeContentForTags(itr->second.content, itr->second.tags);
  content_map_.erase(itr);

  return true;
}

bool
DataHandler::hasContent(const Content::Ptr& content) const
{
  return content.get() != nullptr && content_map_.find(content->id()) != content_map_.end();
}

bool
DataHandler::hasContent(const toolbox::UID& id) const
{
  return content_map_.find(id) != content_map_.end();
}

Content::Ptr
DataHandler::contentFromID(const toolbox::UID& id) const
{
  auto itr = content_map_.find(id);
  return itr == content_map_.end() ? Content::Ptr() : itr->second.content;
}

bool
DataHandler::relate(const Content::Ptr& content, const std::set<Tag::Ptr>& tags)
{

}

bool
DataHandler::relate(const Relation& relation)
{

}

}
