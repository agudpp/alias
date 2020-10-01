#ifndef QT_CLIENT_KEY_TRIGGER_H_
#define QT_CLIENT_KEY_TRIGGER_H_

#include <memory>

#include <QKeyEvent>


namespace qt_client {

/**
 * @brief Helper class to trigger an action when a key combination is triggered
 */
class KeyTrigger
{
  public:

    typedef std::unique_ptr<KeyTrigger> Ptr;

    struct Configuration {
        Configuration(int the_key = 0,
                      unsigned int the_modifiers_mask = -0,
                      unsigned int the_except_modifiers_mask = -0,
                      QEvent::Type the_event_type = QEvent::None) :
          key(the_key)
        , modifiers_mask(the_modifiers_mask)
        , except_modifiers_mask(the_except_modifiers_mask)
        , event_type(the_event_type)
        {}

        int key;                             /**< the key id */
        unsigned int modifiers_mask;         /**< modifiers mask that should be present or ~0 for none */
        unsigned int except_modifiers_mask;  /**< modifiers that if present we should not trigger */
        QEvent::Type event_type;             /**< the type of event we will accept or none for all */
    };

  public:
    KeyTrigger(const Configuration& config = Configuration());
    virtual ~KeyTrigger() {};

    /**
     * @brief Sets the configuration
     * @param config the config
     */
    inline void
    setConfiguration(const Configuration& config)
    {
      config_ = config;
    }

    /**
     * @brief Gets the configuration
     * @return the config
     */
    inline const Configuration&
    configuration(void) const
    {
      return config_;
    }

    /**
     * @brief Checks if this trigger should be triggered for the given key event
     * @param key_event the key event
     * @return true if we should, false otherwise
     */
    bool
    shouldTrigger(QKeyEvent* key_event);

    /**
     * @brief This method will execute the action
     * @param key_event the key event that triggered this
     * @return true if we should stop processing further events or false otherwise
     */
    virtual bool
    trigger(QKeyEvent* key_event) = 0;


  private:
    Configuration config_;

};

} // namespace qt_client

#endif // QT_CLIENT_KEY_TRIGGER_H_
