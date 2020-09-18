#ifndef SERVICE_SERVICE_API_H_
#define SERVICE_SERVICE_API_H_

#include <string>
#include <vector>
#include <map>
#include <set>

#include <toolbox/types/id_type.h>

#include <storage/data_storage.h>

#include <data/data_mapper.h>

#include <service/service_defs.h>



namespace service {

class ServiceAPI {
public:


public:
  /**
   * @brief Constructs the service api with the required data
   * @param data_mapper the data manager
   * @param data_storage the data storage to be used
   */
  ServiceAPI(data::DataMapper::Ptr data_mapper,
             storage::DataStorage::Ptr data_storage);
  ~ServiceAPI();


  ////////////////////////////////////////////////////////////////////////////
  // API
  //

  /**
   * @brief Searchs a tag by its name
   * @param name    The name of the tag
   * @param result  The resulting tag reference if found
   * @return true if exists | false otherwise
   */
  bool
  getTagByName(const std::string& name, data::Tag::ConstPtr& result) const;

  /**
   * @brief Search tags by the search context
   * @param context The search context
   * @param result  The resulting list of tags and expanded tags
   * @return true on success | false otherwise
   */
  bool
  searchTags(const SearchContext& context, TagSearchReslut& result) const;

  /**
   * @brief Search content based on the search context
   * @param context The search context
   * @param result  The resulting list of tags and content associated to the search
   * @return true on success | false otherwise
   */
  bool
  searchContent(const SearchContext& context, ContentSearchResult& result) const;

  // TODO:
  // add tag
  // remove tag
  // add element
  // remove element

private:

  /**
   * @brief Will return all the common content from all the given tags
   * @param tags the tags to check the content from
   * @return the set of common content ids belinging to all those tags
   */
  std::set<toolbox::UID>
  getCommonContentIDsFromTags(const std::set<data::Tag::ConstPtr>& tags) const;

  /**
   * @brief Will get the relevant suggested tags for a new query and the current tags and
   *        list of common element ids
   * @param query the query
   * @param current_tags the current tags
   * @param common_elements the current common elements
   * @return the list of suggested new tags
   */
  std::set<data::Tag::ConstPtr>
  getRelevantSuggestions(const std::string& query,
                         const std::set<data::Tag::ConstPtr>& current_tags,
                         const std::set<toolbox::UID>& common_elements) const;

  /**
   * @brief Return the elements from the given ids
   * @param ids the ids
   * @return the set of associated elements
   */
  std::set<data::Content::ConstPtr>
  getContents(const std::set<toolbox::UID>& ids) const;


private:
  data::DataMapper::Ptr data_mapper_;
  storage::DataStorage::Ptr data_storage_;

};

} // namespace service

#endif // SERVICE_SERVICE_API_H_
