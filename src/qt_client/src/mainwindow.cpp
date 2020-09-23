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
#include <service/service_api.h>


namespace qt_client {

/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent, service::ServiceAPI::Ptr service_api) :
  QMainWindow(parent)
, ui(new Ui::MainWindow)
, service_api_(service_api)
{
  ui->setupUi(this);

  setFocusPolicy(Qt::FocusPolicy::WheelFocus);
  setWindowFlags(Qt::WindowType::Dialog);
}

MainWindow::~MainWindow()
{
  delete ui;
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


} // namespace qt_client
