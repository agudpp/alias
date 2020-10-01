#ifndef DATA_METADATA_H_
#define DATA_METADATA_H_

#include <string>

#include <data/content_types.h>


namespace data {


class Metadata {
  public:

    /**
     * @brief operator ==
     * @param other
     * @return
     */
    inline bool
    operator==(const Metadata& other) const;

    /**
     * @brief Get / set type
     * @return
     */
    inline ContentType
    type() const;
    inline void
    setType(ContentType type);

    /**
     * @brief Get / set encrypted flag
     * @return
     */
    inline bool
    encrypted() const;
    inline void
    setEncrypted(bool encrypted);


  private:
    ContentType type_ = ContentType::UNDEFINED;
    bool encrypted_ = false;
};







//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//


inline bool
Metadata::operator==(const Metadata& other) const
{
  return type_ == other.type() && encrypted_ == other.encrypted();
}

inline ContentType
Metadata::type() const
{
  return type_;
}

inline void
Metadata::setType(ContentType type)
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
