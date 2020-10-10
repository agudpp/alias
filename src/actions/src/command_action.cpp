#include <actions/command_action.h>

#include <toolbox/debug/debug.h>
#include <toolbox/os/os_helper.h>


namespace actions {

bool
CommandAction::execute(const data::Content::ConstPtr& content)
{
  LOG_INFO("Executing the Command action: " << content->data());
  return toolbox::OSHelper::execute(content->data(), "", true);
}


} // namespace actions

