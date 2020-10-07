#ifndef SERVICE_SERVICE_API_H_
#define SERVICE_SERVICE_API_H_

#include <string>
#include <memory>
#include <map>
#include <set>

#include <toolbox/types/id_type.h>

#include <storage/data_storage.h>

#include <data/data_mapper.h>

#include <service/service_defs.h>



namespace service {

class ServiceAPI {
public:

    using Ptr = std::shared_ptr<ServiceAPI>;

    struct Config {
        Config(int the_max_number_tag_results = 15) :
          max_number_tag_results(the_max_number_tag_results)
        {}

        /**
         * @brief max_number_tag_results is the maximum number of tags we want to return
         *        per search
         */
        int max_number_tag_results;
    };

public:
  /**
   * @brief Constructs the service api with the required data
   * @param data_mapper the data manager
   * @param data_storage the data storage to be used
   */
  ServiceAPI(data::DataMapper::Ptr data_mapper,
             storage::DataStorage::Ptr data_storage,
             const Config& config = Config());
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
   * @brief Search a list of tags by its ids
   * @param tag_ids The list of tag ids to search for
   * @param tags    The associated tags in the same order than the tag_ids
   * @return true on success | false otherwise
   */
  bool
  getTagsByIds(const std::vector<toolbox::UID>& tag_ids,
               std::vector<data::Tag::ConstPtr>& tags) const;

  /**
   * @brief Returs a content by its id
   * @param content_id  The content id we are searching for
   * @param content     The resulting content object if found
   * @return true if we found it, false otherwise
   */
  bool
  getContentById(const toolbox::UID& content_id, data::Content::ConstPtr& content) const;

  /**
   * @brief Check if we have a content with a given id
   * @param content_id  The content ID we want to check if exists or not
   * @return true if it does, false otherwise
   */
  bool
  hasContentWithId(const toolbox::UID& content_id) const;

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

  //
  // Content and tags creation / handling
  //

  /**
   * @brief Create a tag
   * @param name The name of the tag to be created
   * @return the new tag on success or nullptr otherwise
   */
  data::Tag::ConstPtr
  createTag(const std::string& name);

  /**
   * @brief Updates a tag
   * @param tag_id  The tag id to be updated
   * @param tag     The new tag content
   * @return the tag object updated on success, null on error
   */
  data::Tag::ConstPtr
  updateTag(const toolbox::UID& tag_id, data::Tag::Ptr tag_data);

  /**
   * @brief Deletes a tag and updates all the contents that have that tag
   * @param tag_id  The id of the tag that we want to delete
   * @return true on success | false otherwise
   */
  bool
  deleteTag(const toolbox::UID& tag_id);

  /**
   * @brief Create a new content
   * @param meta_type       The metadata type if any
   * @param meta_encrypted  The metadata encrypted flag
   * @param data            The data content
   * @param tag_ids         The list of tags if any
   * @return the content object on success, nullptr otherwise
   */
  data::Content::Ptr
  createContent(data::ContentType meta_type,
                bool meta_encrypted,
                const std::string& data,
                const std::set<toolbox::UID>& tag_ids = {});

  /**
   * @brief Updates a content
   * @param content_id  The content id we want to update
   * @param content     The content object that will overwrite the old one
   * @return the content object on success, nullptr otherwise
   */
  data::Content::Ptr
  updateContent(const toolbox::UID& content_id, data::Content::Ptr content);

  /**
   * @brief Deletes a content object
   * @param content_id  The id of the content object that we want to delete
   * @return true on success | false otherwise
   */
  bool
  deleteContent(const toolbox::UID& content_id);



private:

  /**
   * @brief Will return all the common content from all the given tags
   * @param tags the tags to check the content from
   * @return the set of common content ids belinging to all those tags
   */
  std::set<toolbox::UID>
  getCommonContentIDsFromTags(const std::vector<data::Tag::ConstPtr>& tags) const;

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
  Config config_;

};

} // namespace service

#endif // SERVICE_SERVICE_API_H_
