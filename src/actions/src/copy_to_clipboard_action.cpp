#include <actions/copy_to_clipboard_action.h>

#include <QGuiApplication>
#include <QClipboard>

#include <toolbox/debug/debug.h>

namespace actions {

bool
CopyToClipboardAction::execute(const data::Content::ConstPtr& content)
{
  LOG_INFO("Copying to clipboard the text: " << content->data());
  QClipboard *clipboard = QGuiApplication::clipboard();
  clipboard->setText(QString::fromStdString(content->data()));
  return true;
}

} // namespace actions
