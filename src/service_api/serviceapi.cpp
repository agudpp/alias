#include "serviceapi.h"

#include <algorithm>
#include <sstream>

#include <core/debug/Debug.h>
#include <consts.h>
#include <tags/tagmanager.h>
#include <elements/elementmanager.h>
#include <elements/element_builder.h>
#include <storage/datastorage.h>


namespace {

///
/// \brief intersects will check if (s1 & s2) != 0
/// \param s1
/// \param s2
/// \return
///
template <typename T>
static bool
intersects(const std::set<T>& s1, const std::set<T>& s2)
{
  for (const T& e1 : s1) {
    if (s2.find(e1) != s2.end()) {
      return true;
    }
  }
  return false;
}


}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

std::set<Tag::ConstPtr>
ServiceAPI::getExistingTags(const std::vector<std::string>& tag_names) const
{
  std::set<Tag::ConstPtr> result;

  for(auto& tag_str : tag_names) {
    const std::string normalized_tag_str = normalizeTagText(tag_str);
    const Tag::ConstPtr t = tag_mngr_->getTag(normalized_tag_str);
    if (t == nullptr) {
      LOG_WARNING("the Tag with string %s was not found", tag_str.c_str());
      continue;
    }
    result.insert(t);
  }

  return result;
}

std::set<core::UID>
ServiceAPI::getCommonElementIDsFromTags(const std::set<Tag::ConstPtr>& tags) const
{
  std::set<core::UID> result;
  for (const Tag::ConstPtr& curr_tag : tags) {
    if (result.empty()) {
      result = curr_tag->elementIDsSet();
    } else {
      std::set<core::UID> tmp;
      std::set_intersection(result.begin(),
                            result.end(),
                            curr_tag->elementIDsSet().begin(),
                            curr_tag->elementIDsSet().end(),
                            std::inserter(tmp, tmp.begin()));
      result = tmp;
    }
  }

  return result;
}

std::set<Tag::ConstPtr>
ServiceAPI::getRelevantSuggestions(const std::string& query,
                                   const std::set<Tag::ConstPtr>& current_tags,
                                   const std::set<core::UID>& common_elements) const
{
  std::set<Tag::ConstPtr> result;

  const std::string norm_query = normalizeTagText(query);
  std::vector<Tag::ConstPtr> suggestions;
  tag_mngr_->getSuggestedTags(norm_query, suggestions);
  for (const Tag::ConstPtr& t : suggestions) {
    // here we will filtered out the Tags that don't have any element in common
    // with the Tags already set by the user
    if (current_tags.empty() ||
       (current_tags.find(t) == current_tags.end() &&
       intersects(t->elementIDsSet(), common_elements))) {
      result.insert(t);
    }
  }

  return result;
}

std::set<Element::ConstPtr>
ServiceAPI::getElements(const std::set<core::UID>& ids) const
{
  std::set<Element::ConstPtr> result;
  for (const core::UID& id : ids) {
    result.insert(element_mngr_->getElement(id));
  }
  return result;
}

std::vector<Tag::Ptr>
ServiceAPI::getOrCreateTags(const std::vector<std::string>& tag_texts)
{
  std::vector<Tag::Ptr> result;
  for (const std::string& tag_text : tag_texts) {
    result.push_back(tag_mngr_->getOrCreate(normalizeTagText(tag_text)));
  }
  return result;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ServiceAPI::ServiceAPI(ElementManager* element_mngr,
                       TagManager* tag_mngr,
                       DataStorage* data_storage) :
  element_mngr_(element_mngr)
, tag_mngr_(tag_mngr)
, data_storage_(data_storage)
{
}

////////////////////////////////////////////////////////////////////////////////
ServiceAPI::~ServiceAPI()
{

}

////////////////////////////////////////////////////////////////////////////////
// API
////////////////////////////////////////////////////////////////////////////////


std::string
ServiceAPI::normalizeTagText(const std::string& text)
{
  // TODO: here will be good if we normalize also special characters or
  // other things.
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  result.erase(std::remove_if(result.begin(), result.end(), [](char c){return std::isspace(c);}), result.end());

  return result;
}

bool
ServiceAPI::search(const SearchOptions& so, SearchResult& result) const
{
  ASSERT_PTR(tag_mngr_);
  ASSERT_PTR(element_mngr_);

  // try to get the current Tags if any
  result.matched_tags = getExistingTags(so.tags);

  // now we need to filter all the resulting items
  std::set<core::UID> common_elem_ids = getCommonElementIDsFromTags(result.matched_tags);

  // now we try to expand the current query to get some possible Tags
  result.expanded_tags = getRelevantSuggestions(so.query, result.matched_tags, common_elem_ids);

  // now we have the associated elements for all the current Tags
  result.matched_tags_results = getElements(common_elem_ids);
  for (const Tag::ConstPtr& exp_tag : result.expanded_tags) {
    // get the intersection for this case if and only if there are some
    // Tags already set
    std::set<core::UID> tmp;
    if (!result.matched_tags.empty()) {
      std::set_intersection(common_elem_ids.begin(),
                            common_elem_ids.end(),
                            exp_tag->elementIDsSet().begin(),
                            exp_tag->elementIDsSet().end(),
                            std::inserter(tmp, tmp.begin()));
    } else {
      tmp = exp_tag->elementIDsSet();
    }
    result.exp_results[exp_tag] = getElements(tmp);
  }

  return true;
}

bool
ServiceAPI::searchTags(const SearchOptions& so, SearchTagsReslut& result) const
{
  ASSERT_PTR(tag_mngr_);
  ASSERT_PTR(element_mngr_);

  result.matched_tags = getExistingTags(so.tags);
  const std::set<core::UID> common_elem_ids = getCommonElementIDsFromTags(result.matched_tags);
  result.expanded_tags = getRelevantSuggestions(so.query, result.matched_tags, common_elem_ids);

  return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::getTags(const SearchTag& st, SearchTagResults& result) const
{
  ASSERT_PTR(tag_mngr_);

  const std::string normPrefix = normalizeTagText(st.prefix);
  std::vector<Tag::ConstPtr> suggestions;
  tag_mngr_->getSuggestedTags(normPrefix, result.tags);
  return true;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::getTagsForElementID(const core::UID& element_id, SearchTagResults& tags) const
{
  ASSERT_PTR(element_mngr_);
  ASSERT_PTR(tag_mngr_);

  auto element = element_mngr_->getElement(element_id);
  if (element.get() == nullptr) {
    return false;
  }
  tags.tags = const_cast<const TagManager*>(tag_mngr_)->getTags(element->tagIDsSet());
  return true;
}

////////////////////////////////////////////////////////////////////////////////
Tag::ConstPtr
ServiceAPI::getTagByText(const std::string& text) const
{
  ASSERT_PTR(tag_mngr_);

  const std::string normalized_tag_str = normalizeTagText(text);
  return tag_mngr_->getTag(normalized_tag_str);
}

////////////////////////////////////////////////////////////////////////////////
Tag::ConstPtr
ServiceAPI::addTag(const std::string& text)
{
  ASSERT_PTR(tag_mngr_);
  const std::string normalized_tag_str = normalizeTagText(text);
  Tag::Ptr tag = tag_mngr_->getTag(normalized_tag_str);
  if (tag.get() == nullptr) {
    tag = tag_mngr_->addTag(Tag(core::UID::generateRandom(), normalized_tag_str));
    data_storage_->saveTag(tag);
  }
  return Tag::ConstPtr(tag);
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::addElement(const ElementData& d)
{
  ASSERT_PTR(element_mngr_);
  ASSERT_PTR(tag_mngr_);
  ASSERT_PTR(data_storage_);

  Element::Ptr elem = d.element;
  if (elem.get() == nullptr) {
    LOG_ERROR("Truing to add a null element, skiping");
    return false;
  }

  if (element_mngr_->hasElement(elem->id())) {
    LOG_ERROR("There is already an element with id %s to be created", elem->id().toStr().c_str());
    return false;
  }

  std::vector<Tag::Ptr> tags = getOrCreateTags(d.tags_text);
  for (Tag::Ptr& curr_tag: tags) {
    elem->addTagID(curr_tag->id());
    curr_tag->addElementID(elem->id());
    data_storage_->saveTag(curr_tag);
  }

  element_mngr_->addElement(elem);

  return data_storage_->saveElement(elem);
}

bool
ServiceAPI::updateElement(const core::UID& id, const ElementData& d)
{
  ASSERT_PTR(element_mngr_);
  ASSERT_PTR(tag_mngr_);
  ASSERT_PTR(data_storage_);

  if (!element_mngr_->hasElement(id)) {
    LOG_ERROR("There is no element with id %s to be updated", id.toStr().c_str());
    return false;
  }

  // TODO OPTIMIZATION: for simplicity we will remove the element and add it again,
  // this can be optimized
  removeElement(id);
  return addElement(d);
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::removeTag(const core::UID& tag_id)
{
  ASSERT_PTR(element_mngr_);
  ASSERT_PTR(tag_mngr_);
  ASSERT_PTR(data_storage_);

  Tag::Ptr tag = tag_mngr_->getTag(tag_id);
  if (tag.get() == nullptr) {
    LOG_ERROR("trying to remove an inexistent tag with id %s", tag_id.toStr().c_str());
    return false;
  }

  bool error = false;
  std::vector<Element::Ptr> elements = element_mngr_->getElements(tag->elementIDsSet());
  for (Element::Ptr& elem : elements) {
    elem->removeTagID(tag_id);
    if (!data_storage_->saveElement(elem)) {
      error = true;
    }
  }

  return error;
}

////////////////////////////////////////////////////////////////////////////////
bool
ServiceAPI::removeElement(const core::UID& elem_id)
{
  ASSERT_PTR(element_mngr_);
  ASSERT_PTR(tag_mngr_);
  ASSERT_PTR(data_storage_);

  Element::Ptr elem = element_mngr_->getElement(elem_id);
  if (elem.get() == nullptr) {
    LOG_ERROR("trying to remove an inexistent element with id %s", elem_id.toStr().c_str());
    return false;
  }

  bool error = false;
  std::vector<Tag::Ptr> tags = tag_mngr_->getTags(elem->tagIDsSet());
  for (Tag::Ptr& tag: tags) {
    tag->removeElementID(elem_id);
    if (!data_storage_->saveTag(tag)) {
      error = true;
    }
  }

  error = error || !element_mngr_->removeElement(elem_id);
  error = error || !data_storage_->removeElement(elem);

  return error;
}
