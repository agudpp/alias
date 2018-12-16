#ifndef FUNCTION_KEY_TRIGGER_H
#define FUNCTION_KEY_TRIGGER_H

#include <functional>

#include <QKeyEvent>

#include <ui_client/utils/key_trigger.h>

class FunctionKeyTrigger : public KeyTrigger {
  public:
    FunctionKeyTrigger(const Configuration& config = Configuration()) {}
    FunctionKeyTrigger(const Configuration& config,
                       const std::function<bool (QKeyEvent*)>& f) :
      KeyTrigger(config)
    , func_(f)
    {}
    virtual ~FunctionKeyTrigger(void) {}

    inline void
    setFunction(const std::function<bool (QKeyEvent*)>& fun)
    {
      func_ = fun;
    }

    virtual bool
    trigger(QKeyEvent* key_event) override
    {
      return func_(key_event);
    }

  private:
    std::function<bool (QKeyEvent*)> func_;
};

#endif // FUNCTION_KEY_TRIGGER_H
