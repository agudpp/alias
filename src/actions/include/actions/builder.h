#ifndef ACTIONS_BUILDER_H_
#define ACTIONS_BUILDER_H_

#include <actions/action_defs.h>
#include <actions/action_interface.h>

namespace actions {

class Builder
{
  public:

    /**
     * @brief build an action from a given type
     * @param action_type   The action type
     * @return the associted action if exists, null otherwise
     */
    static ActionInterface::Ptr
    build(Type action_type);
};

} // namespace actions

#endif // ACTIONS_BUILDER_H_
