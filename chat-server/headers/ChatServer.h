#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QTcpServer>
#include <QHostAddress>
#include <QDataStream>
#include <map>

class ChatServer : public QTcpServer {
    Q_OBJECT

public:
    ChatServer(QObject *parent = nullptr);
protected:
    void incomingConnection(qintptr socketDescriptor) override;

private:
    std::map<QTcpSocket *, int> m_clients;
    int m_numOfClients = 0; // static?
    QVector<QString> m_onlineUsers;

    // TODO: use std::map
    QHash<QTcpSocket *, QByteArray *> m_buffers; // We need a buffer to store data until block has been completely received
    QHash<QTcpSocket *, qint32 *> m_sizes; // We need to store the size to verify if a block has been received completely

    // Private member functions
    qint32 QByteArrayToQInt32(QByteArray array);

private slots:
    void onNewConnection();
    void onDataIncoming();
    void onDataReceived(QByteArray &receivedData, QTcpSocket *pSenderSocket); // const?
    void onClientDisconnect();

signals:
    void dataReceived(QByteArray &data, QTcpSocket *pSenderSocket); // const?



};

#endif // MYTCPSERVER_H
