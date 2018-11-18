/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2017 by Happen
 *
 *   File  created by agustin on 1.7.2017
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef DATASTORAGE_H
#define DATASTORAGE_H

#include <vector>

#include <elements/element.h>
#include <tags/tag.h>



class DataStorage {
  public:

    DataStorage(void) = default;
    virtual ~DataStorage() {}

    /**
     * @brief Will load all the elements and returned on the vector
     * @param elements the vector to be filled in with the loaded elements
     * @return true on success | false otherwise
     */
    virtual bool
    loadAllElements(std::vector<Element::Ptr>& elements) = 0;

    /**
     * @brief Will load all the tags and returned on the vector
     * @param tags the vector to be filled in with the loaded tags
     * @return true on success | false otherwise
     */
    virtual bool
    loadAllTags(std::vector<Tag::Ptr>& tags) = 0;

    /**
     * @brief Saves an element
     * @param element the element
     * @return true succes | false otherwise
     */
    virtual bool
    saveElement(const Element::Ptr& element) = 0;

    /**
     * @brief removes a element from the db
     * @param element the element
     * @return true on success | false otherwise
     */
    virtual bool
    removeElement(const Element::Ptr& element) = 0;

    /**
     * @brief Saves a tag
     * @param tag the tag
     * @return true succes | false otherwise
     */
    virtual bool
    saveTag(const Tag::Ptr& tag) = 0;

    /**
     * @brief removes a tag from the db
     * @param tag the tag
     * @return true on success | false otherwise
     */
    virtual bool
    removeTag(const Tag::Ptr& tag) = 0;
};

#endif // DATASTORAGE_H
