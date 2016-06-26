/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 26.6.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef TAGCONTENTMAPPER_H
#define TAGCONTENTMAPPER_H

///
/// \brief The TagContentMapper class will be in charge of mapping all the tags
///        to the different IContent we have. Every IContent that contains a tag
///        will be mapped here from tag->IContent
///
class TagContentMapper
{
public:
    TagContentMapper();
    ~TagContentMapper();

    // load a list of tags (with empty content)

    // check if we have a tag? and return the pointer to it

    // add a new pair (tag, content) (not affect the content here)

    // remove a new (tag, content) pair (not affect the content here)

    // check if there are content with a given tag

    // return all the content from a given tag

    // remove a tag completely (we will not affect the content here)
};

#endif // TAGCONTENTMAPPER_H
