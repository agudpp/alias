#ifndef DATA_UNIQUE_OBJECT_H_
#define DATA_UNIQUE_OBJECT_H_

#include <toolbox/types/id_type.h>

#include <data/metadata.h>


namespace data {

class UniqueObject
{
  public:
    UniqueObject();

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
    inline toolbox::UID&
    id() const;
    inline void
    setID(const toolbox::UID& id);

  private:
    Metadata metadata_;
    toolbox::UID id_;
};




//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//


inline const Metadata&
UniqueObject::metadata() const
{
  return metadata_;
}

inline void
UniqueObject::setMetadata(const Metadata& metadata)
{
  metadata_ = metadata;
}

inline toolbox::UID&
UniqueObject::id() const
{
  return id_;
}

inline void
UniqueObject::setID(const toolbox::UID& id)
{
  id_ = id;
}

} // namespace data

#endif // DATA_UNIQUE_OBJECT_H_
