/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 26.6.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include "IContent.h"

#include <core/debug/Debug.h>


////////////////////////////////////////////////////////////////////////////////
void
IContent::addTag(Tag* tag)
{
    ASSERT_PTR(tag);
    const int index = tagIndexFromPtr(tag);
    if (index >= 0) {
        debugWARNING("We already have the tag with name %s", tag->label().c_str());
        return;
    }
    m_tags.push_back(tag);
}

////////////////////////////////////////////////////////////////////////////////
void
IContent::removeTag(Tag* tag)
{
    ASSERT_PTR(tag);
    const int index = tagIndexFromPtr(tag);
    if (index < 0) {
        debugWARNING("We don't have the tag with name %s", tag->label().c_str());
        return;
    }
    m_tags[index] = m_tags.back();
    m_tags.pop_back();
}

