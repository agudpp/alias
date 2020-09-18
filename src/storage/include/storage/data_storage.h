#ifndef STORAGE_DATASTORAGE_H_
#define STORAGE_DATASTORAGE_H_

#include <vector>
#include <memory>

#include <data/content.h>
#include <data/tag.h>



namespace storage {

class DataStorage {
  public:

    using Ptr = std::shared_ptr<DataStorage>;

  public:

    DataStorage(void) = default;
    virtual ~DataStorage() {}

    /**
     * @brief Will load all the content and returned on the vector
     * @param elements the vector to be filled in with the loaded content
     * @return true on success | false otherwise
     */
    virtual bool
    loadAllContent(std::vector<data::Content::Ptr>& content) = 0;

    /**
     * @brief Will load all the tags and returned on the vector
     * @param tags the vector to be filled in with the loaded tags
     * @return true on success | false otherwise
     */
    virtual bool
    loadAllTags(std::vector<data::Tag::Ptr>& tags) = 0;

    /**
     * @brief Saves a content
     * @param content the content to save
     * @return true succes | false otherwise
     */
    virtual bool
    saveContent(const data::Content::Ptr& content) = 0;

    /**
     * @brief Removes a content from the db
     * @param content The element
     * @return true on success | false otherwise
     */
    virtual bool
    removeContent(const data::Content::Ptr& content) = 0;

    /**
     * @brief Saves a tag
     * @param tag the tag
     * @return true succes | false otherwise
     */
    virtual bool
    saveTag(const data::Tag::Ptr& tag) = 0;

    /**
     * @brief removes a tag from the db
     * @param tag the tag
     * @return true on success | false otherwise
     */
    virtual bool
    removeTag(const data::Tag::Ptr& tag) = 0;
};

} // namespace storage

#endif // STORAGE_DATASTORAGE_H_
