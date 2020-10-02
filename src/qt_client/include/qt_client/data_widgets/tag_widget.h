#ifndef QT_CLIENT_TAG_WIDGET_H_
#define QT_CLIENT_TAG_WIDGET_H_

#include <QWidget>

#include <data/tag.h>


namespace Ui {
class TagWidget;
}


namespace qt_client {

class TagWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TagWidget(QWidget *parent = nullptr,
                       data::Tag::ConstPtr tag = nullptr,
                       bool show_close_button = false);
    virtual ~TagWidget();

    /**
     * @brief setTag
     * @param tag
     */
    void
    setTag(data::Tag::ConstPtr tag);

    /**
     * @brief tag
     * @return
     */
    data::Tag::ConstPtr
    tag();
    const data::Tag::ConstPtr&
    tag() const;

    /**
     * @brief tagName
     * @return
     */
    inline QString
    tagName(void) const
    {
      return tag_->name().c_str();
    }

    /**
     * @brief showCloseButton
     * @param show
     */
    void
    showCloseButton(bool show);

    /**
     * @brief highlight
     */
    void
    highlight(void);

    /**
     * @brief unhighlight
     */
    void
    unhighlight(void);

    /**
     * @brief clear
     */
    void
    reset(void);

  signals:

    /**
     * @brief Signal
     */
    void
    onCloseClicked();

  private:
    Ui::TagWidget *ui;
    data::Tag::ConstPtr tag_;
};

} // namespace qt_client

#endif // QT_CLIENT_TAG_WIDGET_H_
