/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File mainwindow.cpp created by agustin on 29.8.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include "mainwindow.h"

#include <vector>

#include <core/debug/Debug.h>

#include <QStringListModel>
#include <QDebug>
#include <QClipboard>
#include <QDesktopWidget>

#include "ui_mainwindow.h"



/////////////////////////////////////////////////////////////////////////////////

bool
MainWindow::eventFilter(QObject *object, QEvent *event)
{
//  if (event->type() == QEvent::KeyPress) {
//    QKeyEvent *ke = static_cast<QKeyEvent *>(event);
//    if (ke->key() == Qt::Key_Escape) {
//       // special Esc handling here {
//      hideNow();
//    }
//  }
  return false;
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent, ServiceAPI* service_api) :
  QMainWindow(parent)
, ui(new Ui::MainWindow)
, service_api_(service_api)
, tag_handler_(nullptr)
{
  ASSERT_PTR(service_api_);
  ui->setupUi(this);

  setFocusPolicy(Qt::FocusPolicy::WheelFocus);
  setWindowFlags(Qt::WindowType::Dialog);

  ui->resultList->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  // tag handler
  tag_handler_ = new TagHandlerWidget();
  ui->verticalLayout->addWidget(tag_handler_);
  QObject::connect(tag_handler_, &TagHandlerWidget::inputTextChanged,
           this, &MainWindow::tagHandlerInputTextChanged);
  QObject::connect(tag_handler_, &TagHandlerWidget::tagRemoved,
           this, &MainWindow::tagHandlerTagRemoved);
  QObject::connect(tag_handler_, &TagHandlerWidget::tagSelected,
           this, &MainWindow::tagHandlerTagSelected);
  QObject::connect(tag_handler_, &TagHandlerWidget::escapePressed,
           this, &MainWindow::tagHandlerEscapePressed);

  this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
  delete ui;
}


/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::showNow(void)
{
  // clear the old text if any
//  ui->lineEdit->clear();
//  ui->plainTextEdit->clear();

  // put the screen where it should be
  QDesktopWidget *desktop = QApplication::desktop();
  const int WIDTH = 774;
  const int HEIGHT = 220;

  const int screenWidth = desktop->width();
  const int screenHeight = desktop->height();
  const int x = (screenWidth - WIDTH) / 2;
  const int y = (screenHeight - HEIGHT) / 5;

  resize(WIDTH, HEIGHT);
  move( x, y );

  setWindowFlags(Qt::WindowStaysOnTopHint);
  raise();
  show();
  activateWindow();
//  ui->lineEdit->setFocus();
}

/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::hideNow(void)
{
  hide();
  qDebug() << "hidding";
}

/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::showEvent(QShowEvent *e)
{
//  QMainWindow::showEvent(e);
//  // focus lineedit
//  ui->lineEdit->setFocus();
}



void
MainWindow::tagHandlerInputTextChanged(const QString& text)
{
  qDebug() << "Text changed: " << text;
  ServiceAPI::SearchOptions search_options;
  ServiceAPI::SearchResult results;
  search_options.query = text.toStdString();
  search_options.tags = tag_handler_->selectedTagsTexts();

  service_api_->search(search_options, results);

  qDebug() << "Tag results: " << results.matched_tags.size();
  for (Tag::ConstPtr tag : results.matched_tags) {
    qDebug() << "New tag matched: " << tag->text().c_str();
  }

  qDebug() << "Expanded Tag results: " << results.expanded_tags.size();
  for (Tag::ConstPtr tag : results.expanded_tags) {
    qDebug() << "expanded_tags: " << tag->text().c_str();
  }

}

void
MainWindow::tagHandlerTagRemoved(TagWidget* tag)
{

}

void
MainWindow::tagHandlerTagSelected(TagWidget* tag)
{

}

void
MainWindow::tagHandlerEscapePressed(void)
{

}

