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

#include <QStringListModel>
#include <QDebug>
#include <QClipboard>
#include <QDesktopWidget>

#include "ui_mainwindow.h"



/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::highlighted(const QString& item)
{
    Q_ASSERT(m_contentHash != 0);
    qDebug() << "highlighted called: " << item;
    // check if we have the item
    auto it = m_contentHash->find(item);
    if (it == m_contentHash->end()) {
        ui->plainTextEdit->setPlainText("cannot find this element?");
        return;
    }

    // we found it -> set it
    ui->plainTextEdit->setPlainText(it.value());
}

/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::activated(const QString& item)
{
    Q_ASSERT(m_contentHash != 0);

    qDebug() << "activated called: " << item;
    // check if we have the item
    auto it = m_contentHash->find(item);
    if (it == m_contentHash->end()) {
        ui->plainTextEdit->setPlainText("cannot find this element?");
        return;
    }

    // copy to clipboard
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(it.value());

    hideNow();
}

bool
MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *ke = static_cast<QKeyEvent *>(event);
        if (ke->key() == Qt::Key_Escape) {
           // special Esc handling here {
            hideNow();
        }
    }
    return false;
}


/////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
,   m_completer(0)
,   m_contentHash(0)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::FocusPolicy::WheelFocus);
    setWindowFlags(Qt::WindowType::Dialog);

    this->installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::configure(QCompleter* completer, QHash<QString, QString>* hash)
{
    Q_ASSERT(completer != 0);
    Q_ASSERT(hash != 0);

    m_completer = completer;
    m_contentHash = hash;
    if (ui->lineEdit->completer() != completer) {
        ui->lineEdit->setCompleter(m_completer);
        QObject::connect(m_completer, SIGNAL(highlighted(QString)), this, SLOT(highlighted(QString)));
        QObject::connect(m_completer, SIGNAL(activated(QString)), this, SLOT(activated(QString)));
    }
}

/////////////////////////////////////////////////////////////////////////////////
void
MainWindow::showNow(void)
{
    // clear the old text if any
    ui->lineEdit->clear();
    ui->plainTextEdit->clear();

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
    ui->lineEdit->setFocus();
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
    // focus lineedit
    ui->lineEdit->setFocus();
}

