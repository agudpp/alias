#include <toolbox/os/os_helper.h>


namespace toolbox {

namespace OSHelper {

int
execute(const std::string& command, const std::string& arguments)
{
  std::stringstream ss;
  ss << command << " " << arguments;
  const std::string cmd = ss.str();
  return std::system(cmd.c_str()) == 0;
}

int
killByName(const std::string& name)
{
  return execute("killall -e", name);
}

bool
deleteFolder(const std::string& folder)
{
  std::stringstream ss;
  ss << "rm -rf " << folder;
  const std::string command = ss.str();
  return std::system(command.c_str()) == 0;
}

std::string
getUniqueSystemID(void)
{
  return "TODO_IMPLEMENT_THIS";
}


}

}
