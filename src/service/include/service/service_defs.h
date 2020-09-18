#ifndef SERVICE_SERVICE_DEFS_H_
#define SERVICE_SERVICE_DEFS_H_

#include <string>
#include <set>
#include <map>
#include <vector>

#include <data/content.h>
#include <data/tag.h>


namespace service {

// Search context options
//
struct SearchContext {
  // the current user input
  std::string query;
  // the already setted Tags (if there are some before)
  std::set<data::Tag::ConstPtr> tags;
};

// Tag search result
struct TagSearchReslut {
  // all the Tags we are able to match with the current Tags strings and
  // query
  std::set<data::Tag::ConstPtr> matched_tags;
  // expanded possible Tags from last query
  std::set<data::Tag::ConstPtr> expanded_tags;
};

// Content search result
struct ContentSearchResult {
  // the expanded possible resulting elements for each of the expanded
  // possibilities
  std::map<data::Tag::ConstPtr, std::set<data::Content::ConstPtr> > exp_results;
  // the expanded results for the base case (all the elements for the matched_tags) only
  std::set<data::Content::ConstPtr> matched_tags_results;
};


} // namespace service

#endif // SERVICE_SERVICE_DEFS_H_
