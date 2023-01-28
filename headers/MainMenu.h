#ifndef MAINMENU_H
#define MAINMENU_H

#include "headers/Chatroom.h"

#include <QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    MainMenu(QWidget *parent = nullptr);
    ~MainMenu();

private:
    Ui::MainWindow *ui;

    Chatroom *m_pChatroom;


    void setupConnections();


private slots:
    void onConnectButtonClick();
};
#endif // MAINMENU_H
