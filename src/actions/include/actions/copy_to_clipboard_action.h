#ifndef ACTIONS_COPY_TO_CLIPBOARD_ACTION_H_
#define ACTIONS_COPY_TO_CLIPBOARD_ACTION_H_

#include <actions/action_interface.h>

namespace actions {


class CopyToClipboardAction : public ActionInterface
{
  public:
    CopyToClipboardAction() = default;
    ~CopyToClipboardAction() = default;

    virtual bool
    execute(const data::Content::ConstPtr& content) override;
};

} // namespace actions

#endif // ACTIONS_COPY_TO_CLIPBOARD_ACTION_H_
