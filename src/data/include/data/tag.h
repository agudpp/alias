#ifndef DATA_TAG_H_
#define DATA_TAG_H_

#include <string>
#include <memory>

#include <data/unique_object.h>


namespace data {

class Tag : public UniqueObject
{
  public:

    using Ptr = std::shared_ptr<Tag>;

  public:

    // TODO: we may want to avoid the user to create tags directly?
    static Tag
    create(const std::string& name);

    /**
     * @brief Set / get tag name
     * @return
     */
    inline const std::string&
    name() const;
    inline void
    setName(const std::string& name);


  private:
    // TODO: we may want to avoid the user to create tags directly?
    Tag();

  private:
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

inline void
Tag::setName(const std::string& name)
{
  name_ = name;
}

} // namespace data

#endif // DATA_TAG_H_
