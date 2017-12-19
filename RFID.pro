#-------------------------------------------------
#
# Project created by QtCreator 2017-12-13T08:16:35
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RFID
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    reader.cpp \
    login.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    reader.h \
    login.h \
    client.h

FORMS    += mainwindow.ui \
    login.ui \
    client.ui
