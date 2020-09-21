#ifndef DATA_CONTENT_H_
#define DATA_CONTENT_H_

#include <string>
#include <memory>
#include <set>

#include <toolbox/types/id_type.h>

#include <data/metadata.h>


namespace data {

class Content
{
  public:

    using Ptr = std::shared_ptr<Content>;
    using ConstPtr = std::shared_ptr<const Content>;

  public:

    /**
     * @brief Set / get metadata
     * @return
     */
    inline const Metadata&
    metadata() const;
    inline void
    setMetadata(const Metadata& metadata);

    /**
     * @brief Set / get id
     * @return
     */
    inline const toolbox::UID&
    id() const;
    inline void
    setID(const toolbox::UID& id);

    /**
     * @brief Set / get data
     * @return
     */
    inline const std::string&
    data() const;
    inline void
    setData(const std::string& data);

    /**
     * @brief Return / set the list of tag ids
     * @return the list of tag ids
     */
    inline const std::set<toolbox::UID>&
    tagIDs() const;
    inline void
    setTagIDs(const std::set<toolbox::UID>& tag_ids);
    inline void
    addTag(const toolbox::UID& tag_id);
    inline void
    removeTag(const toolbox::UID& tag_id);

    /**
     * @brief Copy all the fields except the ID from another content object
     * @param other The other content object
     */
    void
    copyFrom(const Content& other);

    /**
     * @brief Creates a copy from the current one and returns a new pointer
     * @return a new allocated instance with the same data than current one
     */
    Ptr
    clonePtr() const;



  private:
    Metadata metadata_;
    toolbox::UID id_;
    std::string data_;
    std::set<toolbox::UID> tag_ids_;
};




//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//


inline const Metadata&
Content::metadata() const
{
  return metadata_;
}

inline void
Content::setMetadata(const Metadata& metadata)
{
  metadata_ = metadata;
}

inline const toolbox::UID&
Content::id() const
{
  return id_;
}

inline void
Content::setID(const toolbox::UID& id)
{
  id_ = id;
}

inline const std::string&
Content::data() const
{
  return data_;
}

inline void
Content::setData(const std::string& data)
{
  data_ = data;
}

inline const std::set<toolbox::UID>&
Content::tagIDs() const
{
  return tag_ids_;
}

inline void
Content::setTagIDs(const std::set<toolbox::UID>& tag_ids)
{
  tag_ids_ = tag_ids;
}

inline void
Content::addTag(const toolbox::UID& tag_id)
{
  tag_ids_.insert(tag_id);
}

inline void
Content::removeTag(const toolbox::UID& tag_id)
{
  tag_ids_.erase(tag_id);
}

void
Content::copyFrom(const Content& other)
{
  setMetadata(other.metadata());
  setData(other.data());
  setTagIDs(other.tagIDs());
}

Content::Ptr
Content::clonePtr() const
{
  Content::Ptr result(new Content());
  result->copyFrom(*this);
  return result;
}


} // namespace data

#endif // DATA_CONTENT_H_
