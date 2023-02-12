#include "headers/ChatServer.h"

#include <iostream>
#include <random>
#include <QTcpSocket>
#include <QDataStream>
#include <QEventLoop>
#include <QDataStream>
#include <QTimer>

ChatServer::ChatServer(QObject *parent)
    : QTcpServer(parent)
{}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
    std::cout << "We have a new connection! Client with socketDescriptor " << socketDescriptor << std::endl;

    ClientThread *thread = new ClientThread(socketDescriptor, this);
    connect(thread, &ClientThread::finished, thread, &ClientThread::deleteLater);
    connect(thread, &ClientThread::newConnection, this, &ChatServer::onNewConnection);


    connect(thread, &ClientThread::newMessage, this, &ChatServer::onDataReceived, Qt::DirectConnection);
    connect(this, &ChatServer::messageReadyToBeSentToOtherClients, thread, &ClientThread::onNewMessage, Qt::DirectConnection);

    thread->start();

    // If we emit a signal for a new connection here, server somehow shuts itself down after first client connects.

}

void ChatServer::onNewConnection(qintptr newID)
{
    QByteArray buffer;
    QDataStream outDataStream(&buffer, QIODevice::WriteOnly);
    outDataStream.setVersion(QDataStream::Qt_5_15);
    outDataStream << QString::fromStdString("NEW_CONNECTION")
                  << newID;

    emit messageReadyToBeSentToOtherClients(buffer);     // QByteArray * ?
}

void ChatServer::onClientDisconnect()
{
    std::cout << "We are in onDisconnect but we do nothing here yet" << std::endl;
}

void ChatServer::onDataIncoming() {
//    std::cout << "We are in data incoming slot" << std::endl;

//    QTcpSocket *pSocket = static_cast<QTcpSocket*>(sender());

//    QByteArray *pBuffer = m_buffers.value(pSocket);
//    qint32 *s = m_sizes.value(pSocket);
//    qint32 size = *s;

//    std::cout << "Size: " << size << std::endl;

//    // pBuffer = size | data
//    while(pSocket->bytesAvailable() > 0) // Check if there are any bytes awaiting to be read
//    {
//        // Append the byte array returned by readAll to our buffer
//        pBuffer->append(pSocket->readAll());

//        // In this loop we ensure that we get complete data
//        while((size == 0 && pBuffer->size() >= 4) || (size > 0 && pBuffer->size() >= size)) //While we can process data, process it
//        {
//            if (size == 0 && pBuffer->size() >= 4) // If the data's size has been received completely, we store it
//            {
//                // QByteArray::mid(pos, len) returns a byte array with length `len` starting from position `pos`
//                size = QByteArrayToQInt32(pBuffer->mid(0, 4));
//                *s = size;
//                pBuffer->remove(0, 4);
//            }

//            if (size > 0 && pBuffer->size() >= size) // If the data has been received completely, we store it
//            {
//                QByteArray data = pBuffer->mid(0, size);
//                pBuffer->remove(0, size);
//                size = 0;
//                *s = size;

//                qDebug() << "EMITTING DATARECEIVED...";

//                emit dataReceived(data/*, pSocket*/);
//            }
//        }
//    }
}

void ChatServer::onDataReceived(QByteArray receivedData)
{
    emit messageReadyToBeSentToOtherClients(receivedData);
}

qint32 ChatServer::QByteArrayToQInt32(QByteArray source)
{
    qint32 tmp;
    QDataStream stream(&source, QIODevice::ReadWrite);
    stream >> tmp;
    return tmp;
}

