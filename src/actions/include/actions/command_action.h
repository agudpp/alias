#ifndef ACTIONS_COMMAND_ACTION_H_
#define ACTIONS_COMMAND_ACTION_H_

#include <actions/action_interface.h>


namespace actions {

class CommandAction : public ActionInterface
{
  public:
    CommandAction() = default;
    virtual ~CommandAction() = default;

    virtual bool
    execute(const data::Content::ConstPtr& content) override;

};

} // namespace actions

#endif // ACTIONS_COMMAND_ACTION_H_
