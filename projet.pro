QT       += core gui sql serialport

QT += serialport
QT += core gui sql serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QT += charts
QT += core gui network
QT       += core gui charts
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduino.cpp \
    client.cpp \
    connexion.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    arduino.h \
    client.h \
    connexion.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    ../projet.qrc
