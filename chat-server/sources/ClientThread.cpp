#include "headers/ClientThread.h"

#include <QDataStream>


ClientThread::ClientThread(int socketDescriptor, QObject *parent)
    : QThread(parent),
      m_socketDescriptor(socketDescriptor)
{}

void ClientThread::run()
{
    // QMutexLocker ?


    // TODO: delete tcpsocket ?
    m_pTcpSocket  = new QTcpSocket();
    if(!m_pTcpSocket ->setSocketDescriptor(m_socketDescriptor)) {
        emit error(m_pTcpSocket ->error());
        return;
    }

    connect(m_pTcpSocket , &QAbstractSocket::readyRead, this, &ClientThread::onReadyRead, Qt::DirectConnection);
    connect(m_pTcpSocket, &QAbstractSocket::disconnected, this, &ClientThread::onDisconnect);

    QByteArray buffer;
    QDataStream outDataStream(&buffer, QIODevice::WriteOnly);
    outDataStream.setVersion(QDataStream::Qt_5_15);
    outDataStream << QString::fromStdString("IDENTIFICATION")
                  << qintptr(m_socketDescriptor);
    m_pTcpSocket->write(buffer);

    // Block the calling thread until we finish writing here
    m_pTcpSocket->waitForBytesWritten();

    exec();
}

void ClientThread::onReadyRead()
{
    // TODO: How exactly is this triggered by clicking Send message button in GUI?


    // TODO: This was originally done in ChatServer.cpp in its onDataIncoming()
    qDebug() << "onReadyRead slot test";


    QByteArray data;
    data.append(m_pTcpSocket->readAll());

//    m_pTcpSocket->write(data);
//    m_pTcpSocket->waitForBytesWritten();

    emit newMessage(data);
}
#include <iostream>
void ClientThread::onNewMessage(QByteArray data)
{
    std::cout << "CLIENT_" << m_socketDescriptor << ": " << data.toStdString() << std::endl;



    m_pTcpSocket->write(data);

    m_pTcpSocket->waitForBytesWritten(); // WITHOUT THIS?
}

void ClientThread::onDisconnect()
{
    qDebug() << m_socketDescriptor << " disconnected";


    m_pTcpSocket->deleteLater();
    exit(0);
}

