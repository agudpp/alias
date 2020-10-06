#include <actions/builder.h>

#include <toolbox/debug/debug.h>

#include <actions/copy_to_clipboard_action.h>
#include <actions/command_action.h>

namespace actions {

ActionInterface::Ptr
Builder::build(Type action_type)
{
  ActionInterface::Ptr result;
  switch (action_type) {
    case Type::COPY_TO_CLIPBOARD:
      result.reset(new CopyToClipboardAction);
      break;
    case Type::EXECUTE_COMMAND:
      result.reset(new CommandAction);
      break;
    default:
      LOG_WARNING("We do not have the given action type: " << int(action_type));
  }

  return result;
}



} // namespace actions
