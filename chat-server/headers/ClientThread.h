#ifndef CLIENTTHREAD_H
#define CLIENTTHREAD_H

#include <QThread>
#include <QMutex>
#include <QTcpSocket>


class ClientThread : public QThread {
    Q_OBJECT
public:
    ClientThread(int socketDescriptor, QObject *parent);
protected:
    void run() override;
private:
    int m_socketDescriptor;

signals:
    void error(QTcpSocket::SocketError socketError);
};

#endif // CLIENTTHREAD_H
