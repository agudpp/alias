#ifndef ELEMENT_H
#define ELEMENT_H

#include <string>
#include <set>

#include <rapidjson/document.h>

#include <core/types/id_type.h>

class element
{
public:
    inline element(const core::id_t id, const std::string& t = "");
    ~element() {}

    ///
    /// \brief id
    /// \return
    ///
    inline core::id_t
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
    /// \brief addTagID
    /// \param id
    /// \note all those can be inlined
    ///
    void
    addTagID(core::id_t id);
    void
    removeTagID(core::id_t id);
    bool
    hasTagID(core::id_t id) const;

    ///
    /// \brief tagIDsSet
    /// \return
    ///
    inline const std::set<core::id_t>&
    tagIDsSet(void) const;


    ///
    /// \brief toJSON / fromJSON searialization methods
    /// \return
    ///
    rapidjson::Value
    toJSONValue(rapidjson::Document& d) const;
    std::string
    toJSON(void) const;
    bool
    fromJSONValue(const rapidjson::Value& jo);
    bool
    fromJSON(const std::string& json);


private:
    core::id_t m_id;
    std::string m_text;
    std::set<core::id_t> m_tagIDs;
};

////////////////////////////////////////////////////////////////////////////////
inline element::element(const core::id_t id, const std::string& t) :
    m_id(id)
,   m_text(t)
{}

inline core::id_t
element::id(void) const
{
    return m_id;
}

inline const std::string&
element::text(void) const
{
    return m_text;
}

inline const std::set<core::id_t>&
element::tagIDsSet(void) const
{
    return m_tagIDs;
}


#endif // ELEMENT_H
