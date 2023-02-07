QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    sources/ClientThread.cpp \
    sources/main.cpp \
    sources/ChatServer.cpp

HEADERS += \
    headers/ChatServer.h \
    headers/ClientThread.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

