#ifndef QT_CLIENT_MAINWINDOW_H_
#define QT_CLIENT_MAINWINDOW_H_

#include <QMainWindow>
#include <QDialog>
#include <QString>

#include <service/service_api.h>

#include <qt_client/handlers/tag_search_widget.h>


namespace Ui {
class MainWindow;
}


namespace qt_client {


struct SessionData;


class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr,
                      service::ServiceAPI::Ptr service_api = nullptr,
                      SessionData* session_data = nullptr);
  ~MainWindow();

    /**
     * @brief showNow
     */
    void
    showNow(void);

    /**
     * @brief hideNow
     */
    void
    hideNow(void);

    /**
     * @brief Clear all current data
     */
    void
    clearAll(void);

  protected:

    /**
     * @brief showEvent
     * @param e
     */
    void
    showEvent(QShowEvent *e);


  private slots:

    /**
     * @brief This will be called whenever the user finished the usage of the
     */
    void
    onUsageDone();

  private:

    /**
     * @brief centerOnScreen
     */
    void
    centerOnScreen();


private:
  Ui::MainWindow *ui;
  TagSearchWidget* tag_search_widget_;
  service::ServiceAPI::Ptr service_api_;
};

} // namespace qt_client {


#endif // QT_CLIENT_MAINWINDOW_H_
