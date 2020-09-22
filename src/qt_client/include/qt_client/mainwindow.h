#ifndef QT_CLIENT_MAINWINDOW_H_
#define QT_CLIENT_MAINWINDOW_H_

#include <QMainWindow>
#include <QDialog>
#include <QString>



namespace Ui {
class MainWindow;
}


namespace qt_client {


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();


private:
  Ui::MainWindow *ui;
};

} // namespace qt_client {


#endif // QT_CLIENT_MAINWINDOW_H_
