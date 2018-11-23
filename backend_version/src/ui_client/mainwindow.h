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
#include <QString>

#include <service_api/serviceapi.h>

#include <ui_client/tag/tag_widget.h>
#include <ui_client/tag/tag_handler_widget.h>

#include <ui_client/utils/tag_selection_helper.h>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, ServiceAPI* service_api = nullptr);
    ~MainWindow();


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

    void
    tagHandlerInputTextChanged(const QString& text);
    void
    tagHandlerTagRemoved(TagWidget* tag);
    void
    tagHandlerTagSelected(TagWidget* tag);
    void
    tagHandlerEscapePressed(void);

//    ///
//    /// \brief highlighted
//    /// \param item
//    ///
//    void
//    highlighted(const QString& item);

//    ///
//    /// \brief activated
//    /// \param text
//    ///
//    void
//    activated(const QString & text);


private:
    Ui::MainWindow *ui;
    ServiceAPI* service_api_;
    TagHandlerWidget* tag_handler_;

};

#endif // MAINWINDOW_H
