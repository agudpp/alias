/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File MainWindow.h created by agustin on 11.4.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


public slots:
    void onItemSelected(const QString& str);

protected:
    void focusOutEvent(QFocusEvent *event);
    void hideEvent(QHideEvent *event);
    void changeEvent(QEvent *event);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
