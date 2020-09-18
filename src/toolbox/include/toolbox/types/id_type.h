#ifndef ID_TYPE_H
#define ID_TYPE_H

#include <string>

#include <uuid/uuid.h>

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
  generateNew();
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
UID::operator!=(const UID& other) const
{
  return !(uuid_compare(uuid_, other.value()) == 0);
}

inline bool
UID::operator<(const UID& other) const
{
  return uuid_compare(uuid_, other.value()) < 0;
}

inline bool 
UID::isSet(void) const
{
  return !uuid_is_null(uuid_);
}

inline std::size_t
UID::operator()(const UID& other) const
{
  return std::hash<std::string>()(other.toStr());
}

inline std::string
UID::toStr(void) const
{
  char out[38];
  uuid_unparse(uuid_, out);
  return std::string(out);
}

inline bool
UID::fromStr(const std::string& uuid)
{
    return uuid_parse(uuid.c_str(), uuid_) == 0;
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
  struct hash<toolbox::UID>
  {
    inline std::size_t operator()(const toolbox::UID& uid) const
    {
      return uid(uid);
    }
  };

}


#endif // ID_TYPE_H
