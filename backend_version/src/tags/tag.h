#ifndef TAG_H
#define TAG_H

#include <string>
#include <set>

#include <rapidjson/document.h>

#include <core/types/id_type.h>

class Tag
{
public:
    inline Tag(const core::UID id, const std::string& t = "");
    ~Tag() {}

    ///
    /// \brief id
    /// \return
    ///
    inline const core::UID&
    id(void) const;

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
    void
    addElementID(const core::UID& elemID);
    void
    removeElementID(const core::UID& elemID);
    bool
    hasElementID(const core::UID& elemID) const;

    ///
    /// \brief elementIDsSet
    /// \return
    ///
    inline const std::set<core::UID>&
    elementIDsSet(void) const;

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
,   text_(t)
{}

inline const core::UID&
Tag::id(void) const
{
    return uid_;
}

inline const std::string&
Tag::text(void) const
{
    return text_;
}

void
Tag::addElementID(const core::UID& elemID)
{
    element_ids_.insert(elemID);
}

void
Tag::removeElementID(const core::UID& elemID)
{
    element_ids_.erase(elemID);
}

bool
Tag::hasElementID(const core::UID& elemID) const
{
    return element_ids_.find(elemID) != element_ids_.end();
}


inline const std::set<core::UID>&
Tag::elementIDsSet(void) const
{
    return element_ids_;
}



#endif // TAG_H
