#ifndef STORAGE_DUMMY_STORAGE_H_
#define STORAGE_DUMMY_STORAGE_H_

#include <storage/data_storage.h>


namespace storage {

class DummyStorage : public DataStorage {
  public:

    DummyStorage(void) = default;
    virtual ~DummyStorage() {}

    /**
     * @brief Will load all the content and returned on the vector
     * @param elements the vector to be filled in with the loaded content
     * @return true on success | false otherwise
     */
    virtual bool
    loadAllContent(std::vector<data::Content::Ptr>& content) override { return true; }

    /**
     * @brief Will load all the tags and returned on the vector
     * @param tags the vector to be filled in with the loaded tags
     * @return true on success | false otherwise
     */
    virtual bool
    loadAllTags(std::vector<data::Tag::Ptr>& tags) override { return true; }

    /**
     * @brief Saves a content
     * @param content the content to save
     * @return true succes | false otherwise
     */
    virtual bool
    saveContent(const data::Content::Ptr& content) override { return true; }

    /**
     * @brief Removes a content from the db
     * @param content The element
     * @return true on success | false otherwise
     */
    virtual bool
    removeContent(const data::Content::Ptr& content) override { return true; }

    /**
     * @brief Saves a tag
     * @param tag the tag
     * @return true succes | false otherwise
     */
    virtual bool
    saveTag(const data::Tag::Ptr& tag) override { return true; }

    /**
     * @brief removes a tag from the db
     * @param tag the tag
     * @return true on success | false otherwise
     */
    virtual bool
    removeTag(const data::Tag::Ptr& tag) override { return true; }
};
}

#endif // STORAGE_DUMMY_STORAGE_H_
