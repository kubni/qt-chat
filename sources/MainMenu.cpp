#include "headers/MainMenu.h"
#include "ui_mainwindow.h"

MainMenu::MainMenu(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setupConnections();
}

MainMenu::~MainMenu()
{
    delete ui;
    delete m_pChatroom;
}

void MainMenu::setupConnections()
{
    connect(ui->btnConnect, &QPushButton::clicked, this, &MainMenu::onConnectButtonClick);
}

void MainMenu::onConnectButtonClick()
{
    ui->btnConnect->setEnabled(false);

    // Parse the address and the port
    QString address = ui->leAddress->text().trimmed();
    int port = ui->lePort->text().trimmed().toInt();
    QString username = ui->leUsername->text().trimmed();
    // Show the chatroom
    this->hide();
    m_pChatroom = new Chatroom(address, port, username);
}



