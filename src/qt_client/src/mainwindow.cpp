#include <qt_client/mainwindow.h>

#include <vector>

#include <QMessageBox>
#include <QKeyEvent>
#include <QStringListModel>
#include <QDebug>
#include <QClipboard>
#include <QDesktopWidget>

#include "ui_mainwindow.h"

// TODO: remove this
#include <qt_client/data_widgets/tag_list_widget.h>
#include <qt_client/common/widget_line_edit.h>
#include <qt_client/handlers/tag_search_widget.h>
#include <qt_client/handlers/tagger_widget.h>
#include <qt_client/handlers/content_editor_widget.h>
#include <service/service_api.h>


namespace qt_client {


void
MainWindow::onUsageDone()
{
  hideNow();
}

void
MainWindow::centerOnScreen()
{
  QDesktopWidget* desktop_widget = QApplication::desktop();
  const QRect screen_geometry = desktop_widget->screenGeometry(desktop_widget->screenNumber(QCursor::pos()));
  const int x = (screen_geometry.width() - width()) / 2;
  const int y = (screen_geometry.height() - height()) / 2;

  move(x + screen_geometry.left(), y + screen_geometry.top());
}

void
MainWindow::showEvent(QShowEvent *e)
{
  QMainWindow::showEvent(e);
  tag_search_widget_->activate();
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent,
                       service::ServiceAPI::Ptr service_api,
                       SessionData* session_data) :
  QMainWindow(parent)
, ui(new Ui::MainWindow)
, tag_search_widget_(nullptr)
, service_api_(service_api)
{
  ui->setupUi(this);

  tag_search_widget_ = new TagSearchWidget(this, service_api, session_data);
  ui->verticalLayout->addWidget(tag_search_widget_);

  setFocusPolicy(Qt::FocusPolicy::WheelFocus);
  setWindowFlags(Qt::WindowType::Dialog);

  QObject::connect(tag_search_widget_, &TagSearchWidget::usageDone,
                   this, &MainWindow::onUsageDone);
}

MainWindow::~MainWindow()
{
  delete ui;
}

void
MainWindow::showNow(void)
{
  // put the screen where it should be
  centerOnScreen();

  //  setWindowFlags(Qt::WindowStaysOnTopHint);
  raise();
  show();
  activateWindow();
  tag_search_widget_->clearAll();
  tag_search_widget_->activate();
}

void
MainWindow::hideNow(void)
{
  hide();
}

void
MainWindow::clearAll(void)
{
  tag_search_widget_->clearAll();
}



} // namespace qt_client
