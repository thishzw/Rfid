#-------------------------------------------------
#
# Project created by QtCreator 2017-11-29T12:35:10
#
#-------------------------------------------------

QT       += core gui
QT       +=serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RFID
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    rfidreader.cpp \
    rfidreaderthread.cpp

HEADERS  += mainwindow.h \
    rfidreader.h \
    rfidreaderthread.h

FORMS    += mainwindow.ui
