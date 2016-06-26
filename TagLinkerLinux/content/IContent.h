/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 26.6.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef ICONTENT_H
#define ICONTENT_H

#include <vector>

#include "IContentDefs.h"
#include "Tag.h"


///
/// \brief The IContent class define the content interface
///
class IContent
{
public:
    IContent(ContentType type = ContentType::CT_NONE) :
        m_type(type)
    {}
    virtual ~IContent() {}


    ///
    /// \brief setID
    /// \param id
    ///
    inline void
    setID(content_id_t id);

    ///
    /// \brief id
    /// \return
    ///
    inline content_id_t
    id(void) const;

    ///
    /// \brief contentType
    /// \return
    ///
    inline ContentType
    contentType(void) const;

    ///
    /// \brief addTag
    /// \param tag
    ///
    void
    addTag(Tag* tag);

    ///
    /// \brief removeTag
    /// \param tag
    ///
    void
    removeTag(Tag* tag);

    ///
    /// \brief tags
    /// \return
    ///
    inline const std::vector<Tag*>&
    tags(void);


    ////////////////////////////////////////////////////////////////////////////
    // Interfaz to be implemented
    //





private:

    ///
    /// \brief tagIndexFromPtr
    /// \param t
    /// \return
    ///
    inline int
    tagIndexFromPtr(const Tag* t) const;

private:
    content_id_t m_id;
    ContentType m_type;
    std::vector<Tag*> m_tags;

};







////////////////////////////////////////////////////////////////////////////////
// Inline stuff
//


inline int
IContent::tagIndexFromPtr(const Tag* t) const
{
    for (unsigned int i = 0; i < m_tags.size(); ++i) {
        if (m_tags[i] == t) {return int(i);}
    }
    return -1;
}

inline void
IContent::setID(content_id_t id)
{
    m_id = id;
}

inline content_id_t
IContent::id(void) const
{
    return m_id;
}

inline ContentType
IContent::contentType(void) const
{
    return m_type;
}

inline const std::vector<Tag*>&
IContent::tags(void)
{
    return m_tags;
}


#endif // ICONTENT_H
