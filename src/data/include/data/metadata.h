#ifndef DATA_METADATA_H_
#define DATA_METADATA_H_

#include <string>


namespace data {


class Metadata {
  public:

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
    int32_t type_ = -1;
    bool encrypted_ = false;
};







//////////////////////////////////////////////////////////////////////////////////////////
// Inline methods
//


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
