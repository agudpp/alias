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

#include <QKeyEvent>
#include <QStringListModel>
#include <QDebug>
#include <QClipboard>
#include <QDesktopWidget>

#include <ui_client/elements/element_executor.h>
#include <ui_client/elements/editors/element_editor.h>

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

  // element handler
  element_handler_ = new ElementHandler();
  ui->verticalLayout->addWidget(element_handler_);
  element_handler_->setFocusPolicy(Qt::FocusPolicy::NoFocus);

  // tag handler
  tag_handler_ = new TagHandlerWidget(nullptr, service_api);
  ui->verticalLayout->addWidget(tag_handler_);
  QObject::connect(tag_handler_, &TagHandlerWidget::inputTextChanged,
           this, &MainWindow::tagHandlerInputTextChanged);
  QObject::connect(tag_handler_, &TagHandlerWidget::tagRemoved,
           this, &MainWindow::tagHandlerTagRemoved);
  QObject::connect(tag_handler_, &TagHandlerWidget::tagSelected,
           this, &MainWindow::tagHandlerTagSelected);
  QObject::connect(tag_handler_, &TagHandlerWidget::someKeyPressed,
           this, &MainWindow::tagHandlerkeyPressed);

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

  element_handler_->clear();
  tag_handler_->clear();
  tag_handler_->activate();
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
  QMainWindow::showEvent(e);
  tag_handler_->activate();
}

void
MainWindow::clearStatus(void)
{
  tag_handler_->clear();
  element_handler_->clear();
}


void
MainWindow::tagHandlerInputTextChanged(const QString& text)
{
  qDebug() << "MainWindow::tagHandlerInputTextChanged called";
  performSearch(text);
}

void
MainWindow::tagHandlerTagRemoved(Tag::ConstPtr tag)
{
  qDebug() << "MainWindow::tagHandlerTagRemoved called";
  performSearch(tag_handler_->currentText());
}

void
MainWindow::tagHandlerTagSelected(Tag::ConstPtr tag)
{
  qDebug() << "MainWindow::tagHandlerTagSelected called";
  performSearch(tag_handler_->currentText());
}

void
MainWindow::tagHandlerkeyPressed(QKeyEvent* event)
{
  const bool is_control_modifier = event->modifiers() & Qt::ControlModifier;

  if (event->key() == Qt::Key_Up) {
    element_handler_->selectPrev();
  } else if (event->key() == Qt::Key_Down) {
    element_handler_->selectNext();
  } else if (event->key() == Qt::Key_Escape) {
    qDebug() << "MainWindow::tagHandlerEscapePressed called";
    hideNow();
  } else if (event->key() == Qt::Key_Return) {
    if (is_control_modifier) {
      if (editSelected()) {
        qDebug() << "Edited successful";
      } else {
        qDebug() << "Edited cancelled";
      }
    } else {
      if (element_handler_->hasSelected())
        qDebug() << "has selected -> executing it";
      if (executeSelected()) {
        hideNow();
      }
    }
  }
}

void
MainWindow::performSearch(const QString& text)
{
  if (text.isEmpty()) {
    return;
  }
  qDebug() << "Performing search with: " << text;
  ServiceAPI::SearchOptions search_options;
  ServiceAPI::SearchResult results;
  search_options.query = text.toStdString();
  search_options.tags = tag_handler_->selectedTagsTexts();

  service_api_->search(search_options, results);

//  qDebug() << "Tag results: " << results.matched_tags.size();
//  for (Tag::ConstPtr tag : results.matched_tags) {
//    qDebug() << "New tag matched: " << tag->text().c_str();
//  }

//  qDebug() << "Expanded Tag results: " << results.expanded_tags.size();
//  for (Tag::ConstPtr tag : results.expanded_tags) {
//    qDebug() << "expanded_tags: " << tag->text().c_str();
//  }

  tag_handler_->setSuggestedTags(results.expanded_tags);

  std::set<Element::ConstPtr> elements;
  for (auto it = results.exp_results.begin(); it != results.exp_results.end(); ++it) {
    ASSERT_PTR(it->first.get());
    if (it->first.get()->text() == "") {
      continue;
    }
//    qDebug() << "adding elements for tag "
    for (const Element::ConstPtr& e : it->second) {
      qDebug() << "adding element: " << e->getIndexingTest().c_str();
      elements.insert(e);
    }
  }
  element_handler_->setElements(std::vector<Element::ConstPtr>(elements.begin(), elements.end()));
}

std::vector<Tag::ConstPtr>
MainWindow::tagsFromElement(const Element::ConstPtr& element)
{
  ASSERT_PTR(element.get());

  ServiceAPI::SearchTagResults result;

  if (!service_api_->getTagsForElementID(element->id(), result)) {
    debugERROR("Error calling the api ofr getting the tags for an element");
  }

  return result.tags;
}


bool
MainWindow::executeSelected(void)
{
  Element::ConstPtr selected = element_handler_->selected();
  if (selected.get() == nullptr) {
    return false;
  }
  ElementExecutor::Data data;
  data.element = selected;

  return ElementExecutor::execute(data);
}

bool
MainWindow::editSelected(void)
{
  Element::ConstPtr selected = element_handler_->selected();
  if (selected.get() == nullptr) {
    return false;
  }
  Element::Ptr result = selected->clone();
  ElementEditor editor(nullptr, service_api_);

  editor.editElement(result, tagsFromElement(selected));

  const int editor_result = editor.executeEditor();
  if (editor_result == QDialog::Accepted) {
    qDebug() << "Edition accepted, element edited properly";
    ServiceAPI::ElementData elem_data;
    elem_data.element = editor.element();
    elem_data.tags_text = editor.tagTexts();
    if (!service_api_->updateElement(elem_data.element->id(), elem_data)) {
      debugERROR("Error trying to add the edited element");
    }
  } else if (editor_result == QDialog::Rejected) {
    qDebug() << "Edition rejected... do nothing";
    return false;
  }

  return true;
}

//bool
//MainWindow::createNew(Element::ConstPtr to_clone)
//{

//}

