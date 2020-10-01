#include <qt_client/content/content_processor.h>

#include <map>

#include <toolbox/debug/debug.h>
#include <actions/builder.h>

namespace qt_client {

static const std::map<data::ContentType, actions::Type> CONTENT_TO_ACTION_MAP = {
  {data::ContentType::TEXT, actions::Type::COPY_TO_CLIPBOARD}
};

/**
 * @brief Returns an action to be applied for a given content type
 * @param content_type The content type
 * @return the action pointer if we found one, nullptr otherwise
 */
static actions::ActionInterface::Ptr
getAction(data::ContentType content_type)
{
  auto action_type_itr = CONTENT_TO_ACTION_MAP.find(content_type);
  if (action_type_itr == CONTENT_TO_ACTION_MAP.end()) {
    return nullptr;
  }
  return actions::Builder::build(action_type_itr->second);
}


bool
ContentProcessor::process(const data::Content::ConstPtr& content)
{
  actions::ActionInterface::Ptr action = getAction(content->metadata().type());
  if (action.get() == nullptr) {
    LOG_WARNING("We do not have an action for the content with type: " << int(content->metadata().type()));
    return false;
  }
  return action->execute(content);
}


} // namespace qt_client

