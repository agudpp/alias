#ifndef ACTIONS_ACTION_INTERFACE_H_
#define ACTIONS_ACTION_INTERFACE_H_

#include <memory>

#include <actions/action_defs.h>

#include <data/content.h>


namespace actions {


class ActionInterface {
  public:

    using Ptr = std::shared_ptr<ActionInterface>;

  public:

    ActionInterface(Type action_type = Type::UNDEFINED) : type_(action_type) {}
    virtual ~ActionInterface() = default;

    /**
     * @brief Set / get type
     * @return type
     */
    inline Type
    type() const;
    inline void
    setType(Type type);

    /**
     * @brief Executes the action for the given content
     * @param content The content that we want to apply the action
     * @return true on success | false otherwise
     */
    virtual bool
    execute(const data::Content::ConstPtr& content) = 0;


  private:
    Type type_;

};





inline Type
ActionInterface::type() const
{
  return type_;
}

inline void
ActionInterface::setType(Type type)
{
  type_ = type;
}


} // namespace actions

#endif // ACTIONS_ACTION_INTERFACE_H_
