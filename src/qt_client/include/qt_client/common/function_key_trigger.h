#ifndef QT_CLIENT_FUNCTION_KEY_TRIGGER_H_
#define QT_CLIENT_FUNCTION_KEY_TRIGGER_H_

#include <functional>

#include <QKeyEvent>

#include <qt_client/common/key_trigger.h>


namespace qt_client {

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


} // namespace qt_client

#endif // QT_CLIENT_FUNCTION_KEY_TRIGGER_H_

