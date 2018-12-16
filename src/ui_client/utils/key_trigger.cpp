#include <ui_client/utils/key_trigger.h>
#include <QDebug>

KeyTrigger::KeyTrigger(const Configuration& config) :
  config_(config)
{
}

bool
KeyTrigger::shouldTrigger(QKeyEvent* key_event)
{
  const int key = key_event->key();
  if (key != config_.key) {
    return false;
  }
  if (config_.event_type != QEvent::Type::None &&
      key_event->type() != config_.event_type) {
    return false;
  }

  const unsigned int modifiers = key_event->modifiers();
  bool result = (config_.modifiers_mask != ~0u
      ? (config_.modifiers_mask & modifiers) == config_.modifiers_mask
      : true);
  result = result && (config_.except_modifiers_mask != ~0u
                      ? (config_.except_modifiers_mask & modifiers) == 0
                      : true);
  return result;
}
