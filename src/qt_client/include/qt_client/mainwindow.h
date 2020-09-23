#ifndef QT_CLIENT_MAINWINDOW_H_
#define QT_CLIENT_MAINWINDOW_H_

#include <QMainWindow>
#include <QDialog>
#include <QString>

#include <service/service_api.h>



namespace Ui {
class MainWindow;
}


namespace qt_client {


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr, service::ServiceAPI::Ptr service_api = nullptr);
  ~MainWindow();


    // TODO: remove this tests
    void testTags();
    void testTagSearch();

private:
  Ui::MainWindow *ui;
  service::ServiceAPI::Ptr service_api_;
};

} // namespace qt_client {


#endif // QT_CLIENT_MAINWINDOW_H_
