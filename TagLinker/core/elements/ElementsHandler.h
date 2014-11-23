#ifndef ELEMENTSHANDLER_H
#define ELEMENTSHANDLER_H

#include <list>
#include <map>

#include "tag/Tag.h"
#include "content/ContentElement.h"
#include "link/Link.h"
#include "ElementHolder.h"

namespace tl {

// @brief This class will be the main core module where we will interact
//        with the API through this class.
//
class ElementsHandler
{
public:
    ElementsHandler();
    ~ElementsHandler(){};

    ////////////////////////////////////////////////////////////////////////////
    // Element operations
    //

    // @brief Add a new element to the handler.
    //        This class will not handle the memory of the elements
    // @param element       The element to be added with the already
    //                      fresh ID.
    // @return true on success | false of the element couldn't be added
    //         because already exists or internal error
    // @note the element shouldn't be removed
    bool
    addNewElement(Element* element);

    // @brief Get an existent element from ID, if not exists 0 is returned
    // @param id        The element id to be checked
    // @return the associated element | 0 if not exists
    //
    Element*
    getElementByID(ElementID id);
    const Element*
    getElementByID(ElementID id) const;

    // @brief Everytime we modify an element (content of the element) we need
    //        to notify this class to apply the changes every where are needed
    //        (SearchEngine, DBSync, etc).
    // @param element       The element that was modified
    // @return true if we can apply the changes | false otherwise.
    //
    bool
    elementModified(Element* element);

    // @brief Delete an existent element. This method will not delete the
    //        memory of the element.
    // @param element   The element to be deleted
    // @return true if we could delete it | false otherwise
    //
    bool
    removeElement(Element* element);


    ////////////////////////////////////////////////////////////////////////////
    // Tag specific operations

    // @brief This method will be used when we want to tag an Content element
    //        with a particular tag.
    // @param tag       The tag we want to set to the Content element
    // @param cntElem   The content element to be tagged
    // @return true on success | false otherwise
    //
    bool
    tagElement(TagElement* tag, ContentElement* cntElem);

    // @brief Untag a current element removing a particular tag.
    // @param tag       The tag we want to remove from the content element
    // @param cntElem   The content element we want to untag
    // @return true on success | false otherwise
    //
    bool
    untagElement(TagElement* tag, ContentElement* cntElem);

    ////////////////////////////////////////////////////////////////////////////
    // Link operations

    // TODO: this method will be implemented later, iteravely.

    // @brief Link two elements

    // @brief Unlink two elements

private:
    ////////////////////////////////////////////////////////////////////////////

    // @brief Internal methods


private:
    // The element holders
    ElementHolder<ContentElement> m_contentHolder;
    ElementHolder<TagElement> m_tagHolder;
    ElementHolder<LinkElement> m_linkHolder;

    // here we will represent the different relations we can have, it will be
    // simulating the relational DB entries
    // The relations we can have between the data is as next:
    // - ContentTag     -> set of ContentElements
    // - Link           -> set of pairs <ContentElements, ContentElements or
    //                  -> set of pais <TagElement, TagElement>
    // But now we need to know what kind of queries we need to have as well.
    // - get all the content elements
    // - get all the tags elements
    // - get all the links elements
    // - get all the content elements that are associated to a tag.
    // - get all the tags associated to an element.
    // - get all the links associated to a tag.
    // - get all the links associated to an element.



};


}

#endif // ELEMENTSHANDLER_H
