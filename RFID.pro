#-------------------------------------------------
#
# Project created by QtCreator 2017-12-21T23:27:29
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RFID
TEMPLATE = app

SOURCES += main.cpp \
    login.cpp \
    mainwindow.cpp \
    client.cpp \
    pay.cpp

HEADERS  += login.h \
    mainwindow.h \
    client.h \
    pay.h

FORMS    += \
    login.ui \
    mainwindow.ui \
    client.ui \
    pay.ui

LIBS+= "C:/Users/Administrator/Documents/RFID/lib/EPCSDK.lib"
INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.
