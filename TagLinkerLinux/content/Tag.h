/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 26.6.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef TAG_H
#define TAG_H


#include <string>

#include "TagDefs.h"


class Tag
{
public:
    Tag();
    ~Tag();

    ///
    /// \brief setLabel
    /// \param label
    ///
    inline void
    setLabel(const std::string& label);

    ///
    /// \brief label
    /// \return
    ///
    inline const std::string&
    label(void) const;

    ///
    /// \brief setID
    /// \param id
    ///
    inline void
    setID(tag_id_t id);

    ///
    /// \brief id
    /// \return
    ///
    inline tag_id_t
    id(void) const;



private:
    std::string m_label;
    tag_id_t m_id;
};



////////////////////////////////////////////////////////////////////////////////
// inline stuff
//

inline void
Tag::setLabel(const std::string& label)
{
    m_label = label;
}

inline const std::string&
Tag::label(void) const
{
    return m_label;
}

inline void
Tag::setID(tag_id_t id)
{
    m_id = id;
}

inline tag_id_t
Tag::id(void) const
{
    return m_id;
}


#endif // TAG_H
