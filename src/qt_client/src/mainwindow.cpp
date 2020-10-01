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
MainWindow::showEvent(QShowEvent *e)
{
  QMainWindow::showEvent(e);
  tag_search_widget_->activate();
}

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent, service::ServiceAPI::Ptr service_api) :
  QMainWindow(parent)
, ui(new Ui::MainWindow)
, tag_search_widget_(nullptr)
, service_api_(service_api)
{
  ui->setupUi(this);

  tag_search_widget_ = new TagSearchWidget(this, service_api);
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
  QDesktopWidget *desktop = QApplication::desktop();
  const int WIDTH = 774;
  const int HEIGHT = 220;

  const int screenWidth = desktop->width();
  const int screenHeight = desktop->height();
  const int x = (screenWidth - WIDTH) / 2;
  const int y = (screenHeight - HEIGHT) / 5;

  resize(WIDTH, HEIGHT);
  move(x, y);

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


// TODO: remove this tests
void
MainWindow::testTags()
{
  std::vector<data::Tag::Ptr> tags{
    data::Tag::Ptr(new data::Tag("tag-1")),
    data::Tag::Ptr(new data::Tag("second-tag")),
//    data::Tag::Ptr(new data::Tag("third-tag")),
//    data::Tag::Ptr(new data::Tag("fourth-tag")),
  };
  std::vector<TagWidget*> tag_widgets;
  for (auto& t : tags) {
    tag_widgets.push_back(new TagWidget(nullptr, t));
  }

  TagListWidget* tlw = new TagListWidget();
  tlw->pushTags(tag_widgets);
  //ui->verticalLayout_2->addWidget(tlw);

  WidgetLineEdit* wle = new WidgetLineEdit(nullptr, tlw);
  ui->verticalLayout_2->addWidget(wle);

//  delete tlw->popTag();
}

void
MainWindow::testTagSearch()
{
  TagSearchWidget* tsw = new TagSearchWidget(nullptr, service_api_);
  ui->verticalLayout_2->addWidget(tsw);
}

void
MainWindow::testEdition(data::Content::Ptr content)
{
  ContentEditorWidget* tsw = new ContentEditorWidget(nullptr, service_api_);
  tsw->setEditableContent(content);
  ui->verticalLayout_2->addWidget(tsw);
}


} // namespace qt_client
