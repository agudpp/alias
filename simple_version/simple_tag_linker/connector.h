/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 30.8.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <QObject>
#include <QHash>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QCompleter>
#include <QStringListModel>

#include "mainwindow.h"



class Connector : public QObject
{
    Q_OBJECT
public:
    explicit Connector(QObject *parent = 0);

    ///
    /// \brief checkConnection
    /// \return
    ///
    bool
    checkConnection(void);

    ///
    /// \brief startServer
    /// \return
    ///
    bool
    startServer(void);

    ///
    /// \brief createDialog
    /// \return
    ///
    bool
    createDialog(void);

    ///
    /// \brief addTagEntry
    /// \param key
    /// \param value
    ///
    void
    addTagEntry(const QString& key, const QString& value);

signals:

public slots:

protected slots:

    ///
    /// \brief accetNewConn
    ///
    void
    accetNewConn(void);


private:
    QTcpServer m_listener;
    QCompleter* m_completer;
    QHash<QString, QString> m_contentHash;
    MainWindow* m_mainWin;
};

#endif // CONNECTOR_H
