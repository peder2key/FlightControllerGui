#-------------------------------------------------
#
# Project created by QtCreator 2014-12-03T10:27:25
#
#-------------------------------------------------

QT       += core gui
QT       += bluetooth
#QT       += serialport
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FlightControllerGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    checksum.cpp \
    telegram.cpp \
    logindialog.cpp \
    qcustomplot.cpp \
    plotsetup.cpp \
    bluetoothdialog.cpp
    #setupserial.cpp \
    #protocol.cpp

HEADERS  += mainwindow.h \
    checksum.h \
    CommandLibrary.h \
    telegram.h \
    logindialog.h \
    qcustomplot.h \
    plotsetup.h \
    bluetoothdialog.h
   # setupserial.h \
   # protocol.h

FORMS    += mainwindow.ui \
    setupserial.ui \
    logindialog.ui \
    bluetoothdialog.ui
