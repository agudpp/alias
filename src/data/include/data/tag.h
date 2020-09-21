#ifndef DATA_TAG_H_
#define DATA_TAG_H_

#include <string>
#include <ostream>
#include <memory>

#include <toolbox/types/id_type.h>


namespace data {

class Tag
{
  public:

    using Ptr = std::shared_ptr<Tag>;
    using ConstPtr = std::shared_ptr<const Tag>;

  public:

    Tag() = default;
    Tag(const std::string& name) : name_(name) {}

    /**
     * @brief Set / get id
     * @return
     */
    inline const toolbox::UID&
    id() const;
    inline void
    setID(const toolbox::UID& id);

    /**
     * @brief Set / get tag name
     * @return
     */
    inline const std::string&
    name() const;
    inline void
    setName(const std::string& name);


  private:
    toolbox::UID id_;
    std::string name_;
};




//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//

inline const std::string&
Tag::name() const
{
  return name_;
}

inline const toolbox::UID&
Tag::id() const
{
  return id_;
}

inline void
Tag::setID(const toolbox::UID& id)
{
  id_ = id;
}

inline void
Tag::setName(const std::string& name)
{
  name_ = name;
}

static inline std::ostream&
operator<<(std::ostream& out_stream, const Tag& tag)
{
  out_stream << "Tag {uuid: " << tag.id() << ", name: " << tag.name() << "}";
  return out_stream;
}

} // namespace data

#endif // DATA_TAG_H_
