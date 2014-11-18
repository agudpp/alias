#ifndef ELEMENTSHANDLER_H
#define ELEMENTSHANDLER_H

#include <list>

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

    // @brief Add a new element

    // @brief Get an existent element from ID

    // @brief Modify/Update an existent element information

    // @brief Delete an existent element


    ////////////////////////////////////////////////////////////////////////////
    // Tag specific operations

    // @brief Tag an element

    // @brief Untag an element

    ////////////////////////////////////////////////////////////////////////////
    // Link operations

    // @brief Link two elements

    // @brief Unlink two elements


};


}

#endif // ELEMENTSHANDLER_H
