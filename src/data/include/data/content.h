#ifndef DATA_CONTENT_H_
#define DATA_CONTENT_H_

#include <string>
#include <memory>

#include <data/unique_object.h>


namespace data {

class Content : public UniqueObject
{
  public:

    using Ptr = std::shared_ptr<Content>;

  public:

    /**
     * @brief Set / get data
     * @return
     */
    inline const std::string&
    data() const;
    inline void
    setData(const std::string& data);

  private:
    std::string data_;
};




//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//

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

} // namespace data

#endif // DATA_CONTENT_H_
