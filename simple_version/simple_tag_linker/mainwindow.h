/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File mainwindow.h created by agustin on 29.8.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QCompleter>
#include <QHash>
#include <QString>
#include <QTcpServer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    ///
    /// \brief configure
    /// \param completer
    /// \param hash
    ///
    void
    configure(QCompleter* completer, QHash<QString, QString>* hash);

    ///
    /// \brief showNow
    ///
    void
    showNow(void);

    ///
    /// \brief hideNow
    ///
    void
    hideNow(void);


    ///
    /// \brief eventFilter
    /// \param object
    /// \param event
    /// \return
    ///
    bool
    eventFilter(QObject *object, QEvent *event);

protected:

    ///
    /// \brief showEvent
    /// \param e
    ///
    void
    showEvent(QShowEvent *e);

protected slots:

    ///
    /// \brief highlighted
    /// \param item
    ///
    void
    highlighted(const QString& item);

    ///
    /// \brief activated
    /// \param text
    ///
    void
    activated(const QString & text);


private:
    Ui::MainWindow *ui;
    QCompleter* m_completer;
    QHash<QString, QString>* m_contentHash;
};

#endif // MAINWINDOW_H
