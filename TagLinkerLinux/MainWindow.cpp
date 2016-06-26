/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File MainWindow.cpp created by agustin on 11.4.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include "ui_mainwindow.h"

#include "MainWindow.h"

#include <QDebug>
#include <QCompleter>
#include <QClipboard>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFocusPolicy(Qt::StrongFocus);

    // fill the list of words
    QStringList wordList;
    wordList << "alpha" << "omega" << "omicron" << "zeta";
    QCompleter *completer = new QCompleter(wordList, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    ui->lineEdit->setCompleter(completer);

    QObject::connect(completer, SIGNAL(activated(QString)), this, SLOT(onItemSelected(QString)));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void
MainWindow::onItemSelected(const QString& str)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(str);
    close();
}


void
MainWindow::focusOutEvent(QFocusEvent *event)
{
    qDebug() << "focus lost";
    //close();
}

void
MainWindow::hideEvent(QHideEvent *event)
{
    qDebug() << "hide event focus lost";
    //close();
}

void
MainWindow::changeEvent(QEvent *event)
{
    qDebug() << "change event";
    if (event->type() == QEvent::ActivationChange) {
        qDebug() << "activation event changed";
        qDebug() << "Is active window: " << isActiveWindow();
    }
    //close();
}
