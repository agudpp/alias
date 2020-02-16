#ifndef DATA_METADATA_H_
#define DATA_METADATA_H_

#include <string>

#include <toolbox/types/time.h>


namespace data {


class Metadata {
  public:

    /**
     * @brief Get / set update time
     * @return
     */
    inline const toolbox::Time&
    updatedTime() const;
    inline void
    setUpdatedTime(const toolbox::Time& updated_time);

    /**
     * @brief Get / set type
     * @return
     */
    inline int32_t
    type() const;
    inline void
    setType(int32_t type);

    /**
     * @brief Get / set encrypted flag
     * @return
     */
    inline bool
    encrypted() const;
    inline void
    setEncrypted(bool encrypted);


  private:
    toolbox::Time updated_time_;
    int32_t type_ = -1;
    bool encrypted_ = false;
};







//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//

inline const toolbox::Time&
Metadata::updatedTime() const
{
  return updated_time_;
}

inline void
Metadata::setUpdatedTime(const toolbox::Time& updated_time)
{
  updated_time_ = updated_time;
}

inline int32_t
Metadata::type() const
{
  return type_;
}

inline void
Metadata::setType(int32_t type)
{
  type_ = type;
}

inline bool
Metadata::encrypted() const
{
  return encrypted_;
}

inline void
Metadata::setEncrypted(bool encrypted)
{
  encrypted_ = encrypted;
}


} // namespace data

#endif // DATA_METADATA_H_
