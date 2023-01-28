#include <QApplication>

#include "headers/ChatServer.h"

int main(int argc, char **argv) {
  QApplication app(argc, argv);
  ChatServer server;

  QHostAddress addr(QString::fromStdString("localhost"));

  if(!server.listen(addr, 8090))
      return -1;

  return app.exec();
}

