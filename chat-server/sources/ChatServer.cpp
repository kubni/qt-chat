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
{
    connect(this, &ChatServer::dataReceived, this, &ChatServer::onDataReceived);
}

void ChatServer::incomingConnection(qintptr socketDescriptor)
{
   std::cout << "We have a new connection! Client with socketDescriptor " << socketDescriptor << std::endl;
   ClientThread *thread = new ClientThread(socketDescriptor, this);
   connect(thread, &ClientThread::finished, thread, &ClientThread::deleteLater);
   connect(this, &ChatServer::newMessage, thread, &ClientThread::onNewMessage);

   // Add the client to the vector of clients:
   m_clients.push_back(thread);


   thread->start();
}

void ChatServer:: onNewConnection()
{
//    while(m_pServer->hasPendingConnections())
//    {
//        std::cout << "We have a new connection!" << std::endl;
//        QTcpSocket *clientSocket = m_pServer->nextPendingConnection();
//        connect(clientSocket, &QIODevice::readyRead, this, &ChatServer::onDataIncoming);
//        connect(this, &ChatServer::dataReceived, this, &ChatServer::onDataReceived, Qt::UniqueConnection);
//        connect(clientSocket, &QAbstractSocket::disconnected, this, &ChatServer::onClientDisconnect);

//        // Add the client to the vector of clients:
//        m_clients[clientSocket] = ++m_numOfClients;

//        /* Initialize  the QHash map fields with default values.
//         * This is used when  we send data to ensure everything goes through. */
//        QByteArray *pBuffer = new QByteArray();
//        qint32 *pSize = new qint32(0);
//        m_buffers.insert(clientSocket, pBuffer);
//        m_sizes.insert(clientSocket, pSize);


//        // Tell everyone the new client's ID and username
//        qint32 clientID = m_clients[clientSocket];
//        QString clientUsername = QString::fromStdString("CLIENT_" + std::to_string(clientID));

//        // Work in progress
//        m_onlineUsers.push_back(clientUsername);
//        for(std::pair<QTcpSocket*, int> clientSocket : m_clients)
//        {
//            std::cout << "Id in loop: " << clientSocket.second << std::endl;


//            QByteArray buffer;
//            QDataStream outDataStream(&buffer, QIODevice::WriteOnly);
//            outDataStream.setVersion(QDataStream::Qt_5_15);
//            outDataStream << clientID
//                          << clientUsername
//                          << m_onlineUsers;
//            clientSocket.first->write(buffer);
//        }
//    }
}

void ChatServer::onClientDisconnect()
{
    std::cout << "We are in onDisconnect but we do nothing here yet" << std::endl;
}


void ChatServer::onDataIncoming() {
    std::cout << "We are in data incoming slot" << std::endl;

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
//                emit dataReceived(data, pSocket);
//            }
//        }
//    }
}

void ChatServer::onDataReceived(QByteArray &receivedData, QTcpSocket *pSenderSocket)
{
    // The ORIGINAL data we receive from one client needs to be relayed to the other clients too
//    for(std::pair<QTcpSocket*, int> clientSocket : m_clients)
//    {
//        std::cout << "ClientSocket id : " << clientSocket.second << std::endl;

//        // Send the received data to all other clients
//        if(clientSocket.first != pSenderSocket)
//        {
//            clientSocket.first->write(receivedData);
//        }
//    }
    emit newMessage(receivedData);
}

qint32 ChatServer::QByteArrayToQInt32(QByteArray source)
{
    qint32 tmp;
    QDataStream stream(&source, QIODevice::ReadWrite);
    stream >> tmp;
    return tmp;
}

