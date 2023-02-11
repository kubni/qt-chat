#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTcpSocket>


class ClientThread : public QThread {
    Q_OBJECT
public:
    ClientThread(int socketDescriptor, QObject *parent = nullptr);
protected:
    void run() override;
private:
    int m_socketDescriptor;
    QTcpSocket *m_pTcpSocket;
    QVector<ClientThread *> m_clients;
public slots:
    void onReadyRead();
    void onNewMessage(QByteArray data);
    void onDisconnect();
signals:
    void error(QTcpSocket::SocketError socketError);
    void newMessage(QByteArray data);


};

#endif // CLIENTTHREAD_H
