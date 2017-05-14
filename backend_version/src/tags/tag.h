#ifndef TAG_H
#define TAG_H

#include <string>
#include <set>

#include <core/types/id_type.h>

class tag
{
public:
    inline tag(const core::id_t id, const std::string& t = "");
    ~tag() {}

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
    /// \brief addElementID
    /// \param elemID
    /// \note all those can be inlined
    ///
    void
    addElementID(core::id_t elemID);
    void
    removeElementID(core::id_t elemID);
    bool
    hasElementID(core::id_t elemID) const;

    ///
    /// \brief elementIDsSet
    /// \return
    ///
    inline const std::set<core::id_t>&
    elementIDsSet(void) const;

    ///
    /// \brief toJSON / fromJSON searialization methods
    /// \return
    ///
    std::string
    toJSON(void) const;
    bool
    fromJSON(const std::string& json);

private:
    core::id_t m_id;
    std::string m_text;
    std::set<core::id_t> m_elementIDs;
};





////////////////////////////////////////////////////////////////////////////////
inline tag::tag(const core::id_t id, const std::string& t) :
    m_id(id)
,   m_text(t)
{}

inline core::id_t
tag::id(void) const
{
    return m_id;
}

inline const std::string&
tag::text(void) const
{
    return m_text;
}

inline const std::set<core::id_t>&
tag::elementIDsSet(void) const
{
    return m_elementIDs;
}


#endif // TAG_H
