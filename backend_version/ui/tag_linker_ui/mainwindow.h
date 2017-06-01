#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QVector>
#include <QNetworkAccessManager>

#include "beconn.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void
    setExpandedTags(const QVector<QString>& et);

    void
    setCurrentTags(const QVector<QString>& ct);

    void
    setElements(const QVector<QString>& e);


private:

    ///
    /// \brief connectWithBE
    /// \return
    ///
    bool
    connectWithBE(void);

private:
    Ui::MainWindow *ui;
    QUrl m_url;
    QNetworkAccessManager m_qnam;
    QNetworkReply *m_reply;
    bool m_httpRequestAborted;
};

#endif // MAINWINDOW_H
