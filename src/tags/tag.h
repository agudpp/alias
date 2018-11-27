#ifndef TAG_H
#define TAG_H

#include <string>
#include <set>
#include <memory>

#include <rapidjson/document.h>

#include <core/types/id_type.h>



class Tag {
public:

    typedef std::shared_ptr<Tag> Ptr;
    typedef std::shared_ptr<const Tag> ConstPtr;

public:
  inline Tag(void) {}
  inline Tag(const core::UID id, const std::string& t = "");
  ~Tag() {}

  ///
  /// \brief id
  /// \return
  ///
  inline const core::UID&
  id(void) const;

  ///
  /// \brief Sets the id
  /// \param id the id
  ///
  inline void
  setID(const core::UID& id);

  ///
  /// \brief setText
  /// \param t
  ///
  void
  setText(const std::string& t);

  ///
  /// \brief text
  /// \return
  ///
  inline const std::string&
  text(void) const;

  ///
  /// \brief addElementID
  /// \param elemID
  /// \note all those can be inlined
  ///
  inline void
  addElementID(const core::UID& elemID);
  inline void
  removeElementID(const core::UID& elemID);
  inline bool
  hasElementID(const core::UID& elemID) const;

  ///
  /// \brief elementIDsSet
  /// \return
  ///
  inline const std::set<core::UID>&
  elementIDsSet(void) const;


  /**
   * @brief serialize the current element into the stream
   * @param stream the stream
   * @return true on success false otherwise
   */
  bool
  serialize(std::ostream& stream) const;

  /**
   * @brief deserialize the element from the stream
   * @param stream the stream
   * @return true on success | false otherwise
   */
  bool
  deserialize(std::istream& stream);


  /**
   * @brief operator ==
   * @param other
   * @return
   */
  inline bool
  operator==(const Tag& other) const;

private:

  ///
  /// \brief toJSON / fromJSON searialization methods
  /// \return
  ///
  rapidjson::Value
  toJSONValue(rapidjson::Document& d) const;
  std::string
  toJSON(void) const;
  bool
  fromJSONValue(const rapidjson::Value& v);
  bool
  fromJSON(const std::string& json);

private:
  core::UID uid_;
  std::string text_;
  std::set<core::UID> element_ids_;
};





////////////////////////////////////////////////////////////////////////////////
inline Tag::Tag(const core::UID id, const std::string& t) :
  uid_(id)
, text_(t)
{}

inline const core::UID&
Tag::id(void) const
{
  return uid_;
}

inline void
Tag::setID(const core::UID& id)
{
  uid_ = id;
}


inline const std::string&
Tag::text(void) const
{
  return text_;
}

inline void
Tag::addElementID(const core::UID& elemID)
{
  element_ids_.insert(elemID);
}

inline void
Tag::removeElementID(const core::UID& elemID)
{
  element_ids_.erase(elemID);
}

inline bool
Tag::hasElementID(const core::UID& elemID) const
{
  return element_ids_.find(elemID) != element_ids_.end();
}


inline const std::set<core::UID>&
Tag::elementIDsSet(void) const
{
  return element_ids_;
}

inline bool
Tag::operator==(const Tag& other) const
{
  return uid_ == other.uid_ &&
      text_ == other.text_ &&
      element_ids_ == other.element_ids_;
}


#endif // TAG_H
