#ifndef ELEMENT_EXECUTOR_H
#define ELEMENT_EXECUTOR_H

#include <elements/element.h>


/**
 * @brief Will perform the action on a given element
 */
class ElementExecutor {
  public:

    struct Data {
      Element::ConstPtr element;
    };

  public:

    /**
     * @brief executes the given element
     * @param data the data required to execute the element
     * @return true on success, false otherwise
     */
    static bool
    execute(const Data& data);
};

#endif // ELEMENT_EXECUTOR_H
