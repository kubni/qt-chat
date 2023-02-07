#include "headers/ClientThread.h"

#include <QDataStream>


ClientThread::ClientThread(int socketDescriptor, QObject *parent)
    : QThread(parent),
      m_socketDescriptor(socketDescriptor)

{
}

void ClientThread::run()
{
    // TODO: delete()
    QTcpSocket *pTcpSocket = new QTcpSocket();
    if(!pTcpSocket->setSocketDescriptor(m_socketDescriptor)) {
        emit error(pTcpSocket->error());
        return;
    }

    QByteArray buffer;
    QDataStream outDataStream(&buffer, QIODevice::WriteOnly);
    outDataStream.setVersion(QDataStream::Qt_5_15);
    outDataStream << QString::fromStdString("Hello from the server!");
    pTcpSocket->write(buffer);

    // Block the calling thread until we finish writing here
    pTcpSocket->waitForBytesWritten();
}
