#ifndef TAG_WIDGET_H
#define TAG_WIDGET_H

#include <QWidget>

#include <tags/tag.h>

namespace Ui {
class TagWidget;
}

class TagWidget : public QWidget
{
    Q_OBJECT

  public:
    explicit TagWidget(QWidget *parent = nullptr);
    ~TagWidget();

    void
    configure(const Tag::ConstPtr& tag);

    inline Tag::ConstPtr
    tag(void) const
    {
      return tag_;
    }

    inline QString
    text(void) const
    {
      return tag_->text().c_str();
    }

    void
    highlight(void);

    void
    unhighlight(void);

    void
    clear(void);


  private:
    Ui::TagWidget *ui;
    Tag::ConstPtr tag_;
};

#endif // TAG_WIDGET_H
