#ifndef QT_CLIENT_CONTENT_WIDGET_INTERFACE_H_
#define QT_CLIENT_CONTENT_WIDGET_INTERFACE_H_


#include <QWidget>

#include <data/content.h>


namespace qt_client {


class ContentWidgetInterface : public QWidget
{
  public:
    ContentWidgetInterface(QWidget* parent = nullptr);
    virtual ~ContentWidgetInterface() = default;


    /**
     * @brief Set / get const ref
     * @param ref the const ref
     */
    inline void
    setConstRef(data::Content::ConstPtr ref);
    inline data::Content::ConstPtr
    constRef() const;

    /**
     * @brief Set / get the non-const ref
     * @param ref the non-const ref
     */
    inline void
    setRef(data::Content::Ptr ref);
    inline data::Content::Ptr
    ref();

    /**
     * @brief Will return a copy of either const or editable ref as const ref
     * @return a copy of a const reference either from the const_ref or editable ref
     */
    inline data::Content::ConstPtr
    anyRef() const;

    /**
     * @brief Check if we are able to edit or not the content
     * @return true if we are | false otherwise
     */
    inline bool
    canEdit() const;

    /**
     * @brief This function will be called before storing the changes applied on the
     *        ref (should be edditable).
     *        The changes must be applied to the ref() object
     */
    virtual void
    applyChanges() = 0;


  protected:

    /**
     * @brief This should be overrided by the inherited classes to detect when a new ref is set
     */
    virtual void
    newRefSet() = 0;


  private:
    // We will have both refs depending if we are in edditing mode or viewing mode
    data::Content::ConstPtr const_ref_;
    data::Content::Ptr ref_;
};



inline void
ContentWidgetInterface::setConstRef(data::Content::ConstPtr ref)
{
  const_ref_ = ref;
  newRefSet();
}

inline data::Content::ConstPtr
ContentWidgetInterface::constRef() const
{
  return const_ref_;
}

inline void
ContentWidgetInterface::setRef(data::Content::Ptr ref)
{
  ref_ = ref;
  newRefSet();
}

inline data::Content::Ptr
ContentWidgetInterface::ref()
{
  return ref_;
}

inline data::Content::ConstPtr
ContentWidgetInterface::anyRef() const
{
  return canEdit() ? data::Content::ConstPtr(ref_) : constRef();
}

inline bool
ContentWidgetInterface::canEdit() const
{
  return ref_.get() != nullptr;
}

} // namespace qt_client

#endif // QT_CLIENT_CONTENT_WIDGET_INTERFACE_H_
