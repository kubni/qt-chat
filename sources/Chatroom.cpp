#include "headers/Chatroom.h"
#include "ui_chatroom.h"

#include <QLabel>
#include <QKeyEvent>
#include <QDateTime>
#include <QTimer>
#include <iostream>


Chatroom::Chatroom(QString address, int port, QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::Chatroom),
    m_pTcpSocket(new QTcpSocket(this))
{
    ui->setupUi(this);
    setupConnections();

    // Install an event filter on our text edit so we can react to ENTER key press with Chatroom's eventFilter()
    ui->teMessageInput->installEventFilter(this);
    ui->teMessageInput->setPlaceholderText("Type something...");
    // Setup data stream
    m_inDataStream.setDevice(m_pTcpSocket);
    m_inDataStream.setVersion(QDataStream::Qt_5_15);

    // Connect to the server
    m_pTcpSocket->abort();
    m_pTcpSocket->connectToHost(address, port);
}

Chatroom::~Chatroom()
{
    delete ui;
    delete m_pTcpSocket;
}


bool Chatroom::eventFilter(QObject *obj, QEvent *event)
{
    if(event->type() == QEvent::KeyPress)
    {
        QKeyEvent *pKeyEvent = static_cast<QKeyEvent *>(event);

        // We add pressed keys to the set
        m_pressedKeys += pKeyEvent->key();

        // We only want to send the message if RETURN was clicked, not if SHIFT+RETURN was clicked.
        if (m_pressedKeys.contains(Qt::Key_Return) && !m_pressedKeys.contains(Qt::Key_Shift))
        {
            emit messageSubmitted();
            return true;
        }
    }
    else if (event->type() == QEvent::KeyRelease )
    {
       m_pressedKeys -= (static_cast<QKeyEvent *>(event)->key());
    }

    return QObject::eventFilter(obj, event);
}

void Chatroom::setupConnections()
{
    connect(m_pTcpSocket, &QIODevice::readyRead, this, &Chatroom::onDataIncoming);
    connect(this, &Chatroom::messageSubmitted, this, &Chatroom::onMessageSubmit);
}

bool Chatroom::sendDataToServer(QByteArray &data)
{
    if(m_pTcpSocket->state() == QAbstractSocket::ConnectedState)
    {
        // First we should send the data size so server knows how big is the incoming data
        m_pTcpSocket->write(QInt32ToQByteArray(data.size()));

        // Then we send the actual data
        m_pTcpSocket->write(data);

        return m_pTcpSocket->waitForBytesWritten();
    }
    else
    {
        std::cerr << "Error: the socket that is trying to write the data is in UNCONNECTED state!" << std::endl;
        return false;
    }
}

QByteArray Chatroom::QInt32ToQByteArray(qint32 source)
{
    QByteArray tmp;
    QDataStream stream(&tmp, QIODevice::ReadWrite);
    stream << source;
    return tmp;
}

void Chatroom::onDataIncoming()
{
    std::cout << "In data incoming" << std::endl;
    // Wait for everything to be read
    m_inDataStream.startTransaction();

    // We can read the data here from the stream

    if(!m_inDataStream.commitTransaction())
        return;

}

void Chatroom::onMessageSubmit()
{

    QString currentTime = "[" + QDateTime::currentDateTime().toString("hh:mm:ss") + "] ";
    m_currentMessage = currentTime + m_myUsername + ": " + ui->teMessageInput->toPlainText();

    ui->tbChat->append(m_currentMessage);

    QByteArray buffer;
    QDataStream outDataStream(&buffer, QIODevice::WriteOnly);
    outDataStream.setVersion(QDataStream::Qt_5_15);
    outDataStream << m_myUsername
                  << m_currentMessage;
    sendDataToServer(buffer);
//    blockingLoop.exec();
}
