#ifndef QT_CLIENT_TEXT_CONTENT_WIDGET_H_
#define QT_CLIENT_TEXT_CONTENT_WIDGET_H_

#include <qt_client/content/content_widget_interface.h>



namespace Ui {
class TextContentWidget;
}


namespace qt_client {

class TextContentWidget : public ContentWidgetInterface
{
  public:
    TextContentWidget(QWidget* parent = nullptr);
    virtual ~TextContentWidget();


    virtual void
    applyChanges() override;

    virtual void
    activate() override;


  protected:

    virtual void
    newRefSet() override;


  private:
    Ui::TextContentWidget *ui;
};

} // namespace qt_client

#endif // QT_CLIENT_TEXT_CONTENT_WIDGET_H_
