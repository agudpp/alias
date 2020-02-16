#ifndef DATA_RELATION_H_
#define DATA_RELATION_H_

#include <set>

#include <toolbox/types/id_type.h>

namespace data {

class Relation {
  public:

    /**
     * @brief Get / set the tag id
     * @return
     */
    inline const toolbox::UID&
    tagID() const;
    inline void
    setTagID(const toolbox::UID& id);

    /**
     * @brief Get / set the associated content ids
     * @return
     */
    inline const std::set<toolbox::UID>&
    contentIDs() const;
    inline void
    setContentIDs(const std::set<toolbox::UID>& ids);
    inline void
    addContentID(const toolbox::UID& id);

  private:
    toolbox::UID tag_id_;
    std::set<toolbox::UID> content_ids_;
};



//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//

inline const toolbox::UID&
Relation::tagID() const
{
  return tag_id_;
}

inline void
Relation::setTagID(const toolbox::UID& id)
{
  tag_id_ = id;
}

inline const std::set<toolbox::UID>&
Relation::contentIDs() const
{
  return content_ids_;
}

inline void
Relation::setContentIDs(const std::set<toolbox::UID>& ids)
{
  content_ids_ = ids;
}

inline void
Relation::addContentID(const toolbox::UID& id)
{
  content_ids_.insert(id);
}

} // namespace data

#endif // DATA_RELATION_H_
