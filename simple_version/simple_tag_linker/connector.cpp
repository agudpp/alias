/*
 *******************************************************************************
 * TEMPLATE_TO_BE_CHANGED_BEGIN
 *   Copyright (C) 2016 by Happen
 *
 *   File  created by agustin on 30.8.2016
 * TEMPLATE_TO_BE_CHANGED_END
 *******************************************************************************
 */

#include "connector.h"


#define CONN_LH_PORT    5432



/////////////////////////////////////////////////////////////////////////////////
void
Connector::accetNewConn(void)
{
    qDebug() << "accept new connection called";
    QTcpSocket * socket = m_listener.nextPendingConnection();
    if (socket) {
        //delete socket;
    }
    createDialog();
}


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
Connector::Connector(QObject *parent) :
    QObject(parent)
,   m_completer(0)
,   m_mainWin(new MainWindow(0))
{
    // configure the completer
    QStringList words;
    m_completer = new QCompleter(words,this);
    m_completer->setCaseSensitivity(Qt::CaseInsensitive);
    m_mainWin->configure(m_completer, &m_contentHash);

}


////////////////////////////////////////////////////////////////////////////////
bool
Connector::checkConnection(void)
{
    QTcpSocket socket;
    socket.connectToHost(QHostAddress::LocalHost, CONN_LH_PORT);
    if (!socket.waitForConnected()) {
        qDebug() << "error connecting to the socket";
        return false;
    }

    // we could connect
    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool
Connector::startServer(void)
{
    // start listening on a port
    if (!m_listener.listen(QHostAddress::LocalHost, 5432)) {
        qDebug() << "error listening at QHostAddress::LocalHost, 5432";
        return false;
    }
    QObject::connect(&m_listener, SIGNAL(newConnection()), this, SLOT(accetNewConn()));
    return true;
}


////////////////////////////////////////////////////////////////////////////////
bool
Connector::createDialog(void)
{
    m_mainWin->showNow();
    return true;
}



/////////////////////////////////////////////////////////////////////////////////
void
Connector::addTagEntry(const QString& key, const QString& value)
{
    QStringListModel *model = (QStringListModel*)(m_completer->model());
    if (model == 0) {
        qDebug() << "model is null";
        return;
    }
    model->setStringList(model->stringList() << key);
    m_contentHash[key] = value;
}
