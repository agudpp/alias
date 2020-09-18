#include <service/service_api.h>

#include <algorithm>
#include <sstream>

#include <toolbox/debug/debug.h>


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

template <typename T>
static std::set<toolbox::UID>
convertToIDs(const std::set<T>& items)
{
  std::set<toolbox::UID> result;
  for (auto& item : items) {
    result.insert(item->id());
  }
  return result;
}


///
/// \brief Will normalize a tag text
/// \param text the text to normalize
/// \return the normalized tag text
///
static std::string
normalizeTagText(const std::string& text)
{
  // TODO: here will be good if we normalize also special characters or
  // other things.
  std::string result = text;
  std::transform(result.begin(), result.end(), result.begin(), ::tolower);
  result.erase(std::remove_if(result.begin(), result.end(), [](char c){return std::isspace(c);}), result.end());

  return result;
}

}

namespace service {

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////


std::set<toolbox::UID>
ServiceAPI::getCommonContentIDsFromTags(const std::set<data::Tag::ConstPtr>& tags) const
{
  std::set<toolbox::UID> result;
  for (const data::Tag::ConstPtr& curr_tag : tags) {
    std::set<toolbox::UID> content_ids = convertToIDs(data_mapper_->contentsForTag(curr_tag->id()));
    if (result.empty()) {
      result = content_ids;
    } else {
      std::set<toolbox::UID> tmp;
      std::set_intersection(result.begin(),
                            result.end(),
                            content_ids.begin(),
                            content_ids.end(),
                            std::inserter(tmp, tmp.begin()));
      result = tmp;
    }
  }

  return result;
}

std::set<data::Tag::ConstPtr>
ServiceAPI::getRelevantSuggestions(const std::string& query,
                                   const std::set<data::Tag::ConstPtr>& current_tags,
                                   const std::set<toolbox::UID>& common_elements) const
{
  std::set<data::Tag::ConstPtr> result;

  const std::string norm_query = normalizeTagText(query);
  auto suggestions = data_mapper_->suggestedTags(norm_query);
  for (const data::Tag::ConstPtr& suggested_tag : suggestions) {
    // here we will filtered out the Tags that don't have any element in common
    // with the Tags already set by the user
    if (current_tags.empty() ||
       (current_tags.find(suggested_tag) == current_tags.end() &&
       intersects(convertToIDs(data_mapper_->contentsForTag(suggested_tag->id())), common_elements))) {
      result.insert(suggested_tag);
    }
  }

  return result;
}

std::set<data::Content::ConstPtr>
ServiceAPI::getContents(const std::set<toolbox::UID>& ids) const
{
  std::set<data::Content::ConstPtr> result;
  for (const toolbox::UID& id : ids) {
    result.insert(data_mapper_->contentFromID(id));
  }
  return result;
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ServiceAPI::ServiceAPI(data::DataMapper::Ptr data_mapper,
                       storage::DataStorage::Ptr data_storage) :
  data_mapper_(data_mapper)
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

bool
ServiceAPI::getTagByName(const std::string& name, data::Tag::ConstPtr& result) const
{
  result = data_mapper_->tagFromName(name);
  return result.get() != nullptr;
}

bool
ServiceAPI::searchTags(const SearchContext& context, TagSearchReslut& result) const
{
  result.matched_tags = context.tags;
  const std::set<toolbox::UID> common_content_ids = getCommonContentIDsFromTags(context.tags);
  result.expanded_tags = getRelevantSuggestions(context.query, result.matched_tags, common_content_ids);

  return true;
}

bool
ServiceAPI::searchContent(const SearchContext& context, ContentSearchResult& result) const
{
  const std::set<toolbox::UID> common_content_ids = getCommonContentIDsFromTags(context.tags);
  std::set<data::Tag::ConstPtr> expanded_tags =
      getRelevantSuggestions(context.query, context.tags, common_content_ids);

  // now we have the associated elements for all the current Tags
  result.matched_tags_results = getContents(common_content_ids);
  for (const data::Tag::ConstPtr& exp_tag : expanded_tags) {
    // get the intersection for this case if and only if there are some
    // Tags already set
    std::set<toolbox::UID> tmp;
    std::set<toolbox::UID> content_ids = convertToIDs(data_mapper_->contentsForTag(exp_tag->id()));
    if (!context.tags.empty()) {
      std::set_intersection(common_content_ids.begin(),
                            common_content_ids.end(),
                            content_ids.begin(),
                            content_ids.end(),
                            std::inserter(tmp, tmp.begin()));
    } else {
      tmp = content_ids;
    }
    result.exp_results[exp_tag] = getContents(tmp);
  }

  return true;
}




} // namespace service
