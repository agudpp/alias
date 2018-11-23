#ifndef SERVICEAPI_H
#define SERVICEAPI_H

#include <string>
#include <vector>
#include <map>
#include <set>

#include <core/types/basics.h>
#include <core/types/id_type.h>

#include <tags/tag.h>
#include <elements/element.h>


// FWD
class TagManager;
class ElementManager;
class DataStorage;

class ServiceAPI
{
public:

  struct SearchOptions {
    // the current user input
    std::string query;
    // the already setted Tags (if there are some before)
    std::vector<std::string> tags;
  };

  struct SearchResult {
    // all the Tags we are able to match with the current Tags strings and
    // query
    std::set<Tag::ConstPtr> matched_tags;
    // expanded possible Tags from last query
    std::set<Tag::ConstPtr> expanded_tags;
    // the expanded possible resulting elements for each of the expanded
    // possibilities, note that the "" (null Tag) will also contain the possible
    // results for the given matchedTags
    std::map<Tag::ConstPtr, std::set<Element::ConstPtr> > exp_results;
  };

  struct SearchTag {
    // the prefix used to get all the Tags that matches this prefix
    std::string prefix;
  };

  struct SearchTagResults {
    std::vector<const Tag*> tags;
  };

  struct ElementData {
    // the element to be added
    Element::Ptr element;
    // the tags associated to it, will be created if not exists
    std::vector<std::string> tags_text;
  };

public:
  /**
   * @brief Constructs the service api with the required data
   * @param element_mngr the element manager
   * @param tag_mngr the tag manager
   * @param data_storage the data storage to be used
   */
  ServiceAPI(ElementManager* element_mngr,
             TagManager* tag_mngr,
             DataStorage* data_storage);
  ~ServiceAPI();


  ////////////////////////////////////////////////////////////////////////////
  // API
  //

  ///
  /// \brief search
  /// \param so
  /// \param result
  /// \return
  ///
  bool
  search(const SearchOptions& so, SearchResult& result) const;

  ///
  /// \brief getTags
  /// \param st
  /// \param result
  /// \return
  ///
  bool
  getTags(const SearchTag& st, SearchTagResults& result) const;

  ///
  /// \brief addTagElement
  /// \param t
  /// \param e
  /// \return
  ///
  bool
  addElement(const ElementData& d);

  /**
   * @brief Updates an element that is already existent
   * @param id the id of the element
   * @param d the element data
   * @return true if possible | false otherwise
   */
  bool
  updateElement(const core::UID& id, const ElementData& d);

  ///
  /// \brief removeTag
  /// \param TagID
  /// \return
  ///
  bool
  removeTag(const core::UID& tag_id);
  bool
  removeElement(const core::UID& elem_id);

private:

  /**
   * @brief Returns the existing tags we have on the db from a given list of tag names
   * @param tag_names the tag names
   * @return the existing associated tags
   */
  std::set<Tag::ConstPtr>
  getExistingTags(const std::vector<std::string>& tag_names) const;

  /**
   * @brief Will return all the common elements from all the given tags
   * @param tags the tags to check the elements from
   * @return the set of common element ids belinging to all those tags
   */
  std::set<core::UID>
  getCommonElementIDsFromTags(const std::set<Tag::ConstPtr>& tags) const;

  /**
   * @brief Will get the relevant suggested tags for a new query and the current tags and
   *        list of common element ids
   * @param query the query
   * @param current_tags the current tags
   * @param common_elements the current common elements
   * @return the list of suggested new tags
   */
  std::set<Tag::ConstPtr>
  getRelevantSuggestions(const std::string& query,
                         const std::set<Tag::ConstPtr>& current_tags,
                         const std::set<core::UID>& common_elements) const;

  /**
   * @brief Return the elements from the given ids
   * @param ids the ids
   * @return the set of associated elements
   */
  std::set<Element::ConstPtr>
  getElements(const std::set<core::UID>& ids) const;

  /**
   * @brief Return a list of tags alreday existents or creating if not
   * @param tag_texts the text of the tags
   * @return the list of tags
   */
  std::vector<Tag::Ptr>
  getOrCreateTags(const std::vector<std::string>& tag_texts);


private:
  ElementManager* element_mngr_;
  TagManager* tag_mngr_;
  DataStorage* data_storage_;

};

#endif // SERVICEAPI_H
