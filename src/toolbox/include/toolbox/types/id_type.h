#ifndef ID_TYPE_H
#define ID_TYPE_H

#include <ostream>
#include <string>

#include <crossguid/guid.hpp>

namespace toolbox {


class UID {
  public:
    inline UID(void);
    inline UID(const UID& other);
    inline explicit UID(const std::string& str_repr);
    inline UID& operator=(const UID& other) noexcept;
    inline ~UID(void) = default;

    static inline UID generateRandom(void);

    inline void generateNew(void);

    inline bool operator==(const UID& other) const;
    inline bool operator!=(const UID& other) const;

    inline bool operator<(const UID& other) const;

    inline bool isSet(void) const;

    inline std::size_t operator()(const UID& other) const;

    inline std::string toStr(void) const;

    /**
     * @brief Sets the uuid from string
     * @param uuid the string containing the uuid
     * @return true on success, false otherwise
     */
    inline bool fromStr(const std::string& uuid);

  private:
    xg::Guid uuid_;
};



//////////////////////////////////////////////////////////////////////////////////////////
// Inline implementations
//

inline
UID::UID(void)
{
  generateNew();
}

inline
UID::UID(const UID& other) :
  uuid_(other.uuid_)
{  
}

inline
UID& UID::operator=(const UID& other) noexcept
{
  uuid_ = other.uuid_;
  return *this;
}

inline
UID::UID(const std::string& str_repr) :
  uuid_(str_repr)
{
}

inline UID
UID::generateRandom(void)
{
  UID result;
  result.generateNew();
  return result;
}

inline void
UID::generateNew(void)
{
  uuid_ = xg::newGuid();
}


inline bool
UID::operator==(const UID& other) const
{
  return uuid_ == other.uuid_;
}
inline bool
UID::operator!=(const UID& other) const
{
  return uuid_ != other.uuid_;
}

inline bool
UID::operator<(const UID& other) const
{
  return uuid_ < other.uuid_;
}

inline bool 
UID::isSet(void) const
{
  return uuid_.isValid();
}

inline std::size_t
UID::operator()(const UID& other) const
{
  return std::hash<xg::Guid>()(other.uuid_);
}

inline std::string
UID::toStr(void) const
{
  return uuid_.str();
}

inline bool
UID::fromStr(const std::string& uuid)
{
  uuid_ = xg::Guid(uuid);
}


static inline std::ostream&
operator<<(std::ostream& out_stream, const UID& uid)
{
  out_stream << uid.toStr();
  return out_stream;
}


} // namespace toolbox


namespace std {

  template <>
  struct hash<toolbox::UID>
  {
    inline std::size_t operator()(const toolbox::UID& uid) const
    {
      return uid(uid);
    }
  };

}


#endif // ID_TYPE_H
