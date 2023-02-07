#ifndef CHATROOM_H
#define CHATROOM_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QEventLoop>
QT_BEGIN_NAMESPACE
namespace Ui { class Chatroom; }
QT_END_NAMESPACE


class Chatroom : public QMainWindow {
    Q_OBJECT
public:
    Chatroom(QString address, int port, QString username, QWidget *parent = nullptr);
    ~Chatroom();

private:
    Ui::Chatroom *ui;

    bool eventFilter(QObject *obj, QEvent *event) override;
    QTcpSocket *m_pTcpSocket = nullptr;
    QDataStream m_inDataStream;
    qint32 m_myID = -1;
    QString m_myUsername = "";
    QString m_currentMessage;
    QSet<int> m_pressedKeys;

    void setupConnections();
    void displayError(QAbstractSocket::SocketError socketError);
    bool sendDataToServer(QByteArray &dataBuffer);

    QByteArray QInt32ToQByteArray(qint32 source);

private slots:
    void onDataIncoming();
    void onMessageSubmit();

signals:
    void messageSubmitted();

};


#endif // CHATROOM_H
