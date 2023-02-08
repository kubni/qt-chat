#include "headers/ClientThread.h"

#include <QDataStream>


ClientThread::ClientThread(int socketDescriptor, QObject *parent)
    : QThread(parent),
      m_socketDescriptor(socketDescriptor)
{
}

void ClientThread::run()
{
    // TODO: delete tcpsocket ?
    m_pTcpSocket = new QTcpSocket();
    if(!m_pTcpSocket->setSocketDescriptor(m_socketDescriptor)) {
        emit error(m_pTcpSocket->error());
        return;
    }

    QByteArray buffer;
    QDataStream outDataStream(&buffer, QIODevice::WriteOnly);
    outDataStream.setVersion(QDataStream::Qt_5_15);
    outDataStream << QString::fromStdString("IDENTIFICATION")
                  << qintptr(m_socketDescriptor);
    m_pTcpSocket->write(buffer);

    // Block the calling thread until we finish writing here
    m_pTcpSocket->waitForBytesWritten();
}

void ClientThread::onNewMessage(QByteArray &data)
{
    QTcpSocket *pTcpSocket = new QTcpSocket();
    if(!pTcpSocket->setSocketDescriptor(m_socketDescriptor)) {
        emit error(pTcpSocket->error());
        return;
    }
    pTcpSocket->write(data);
}

