#ifndef ID_TYPE_H
#define ID_TYPE_H

#include <string>

#include <uuid/uuid.h>

namespace core {


class UID {
  public:
    inline UID(void);
    inline UID(const UID& other);
    inline UID(const std::string& str_repr);
    inline UID& operator=(const UID& other) noexcept;

    static inline UID generateRandom(void);

    inline void generateNew(void);

    inline bool operator==(const UID& other) const;

    inline bool operator<(const UID& other) const;

    inline std::size_t operator()(const UID& other) const;

    inline std::string toStr(void) const;

    inline void fromStr(const std::string& uuid);

    inline const uuid_t& value(void) const;
    inline uuid_t& value(void);

  private:
    uuid_t uuid_;
};



//////////////////////////////////////////////////////////////////////////////////////////
// Inline implementations
//

inline
UID::UID(void)
{
}

inline
UID::UID(const UID& other)
{
  uuid_copy(uuid_, other.value());
}

inline
UID& UID::operator=(const UID& other) noexcept
{
  uuid_copy(uuid_, other.value());
  return *this;
}

inline
UID::UID(const std::string& str_repr)
{
  fromStr(str_repr);
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
  uuid_generate_random(uuid_);
}


inline bool
UID::operator==(const UID& other) const
{
  return uuid_compare(uuid_, other.value()) == 0;
}

inline bool
UID::operator<(const UID& other) const
{
  return uuid_compare(uuid_, other.value()) < 0;
}

inline std::size_t
UID::operator()(const UID& other) const
{
  return std::hash<const unsigned char*>()(other.value());
}

inline std::string
UID::toStr(void) const
{
  char out[38];
  uuid_unparse(uuid_, out);
  return std::string(out);
}

inline void
UID::fromStr(const std::string& uuid)
{
    uuid_parse(uuid.c_str(), uuid_);
}

inline const uuid_t&
UID::value(void) const
{
  return uuid_;
}

inline uuid_t&
UID::value(void)
{
  return uuid_;
}

}

namespace std {

  template <>
  struct hash<core::UID>
  {
    inline std::size_t operator()(const core::UID& uid) const
    {
      return uid(uid);
    }
  };

}


#endif // ID_TYPE_H
