#include <actions/copy_to_clipboard_action.h>

#include <clip/clip.h>

#include <toolbox/debug/debug.h>

namespace actions {

bool
CopyToClipboardAction::execute(const data::Content::ConstPtr& content)
{
  LOG_INFO("Copying to clipboard the text: " << content->data());
  return clip::set_text(content->data());
}

} // namespace actions
