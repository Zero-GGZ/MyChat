#-------------------------------------------------
#
# Project created by QtCreator 2019-01-24T12:16:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = MyChat
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    login.cpp \
    common.cpp \
    dialog.cpp

HEADERS  += mainwindow.h \
    login.h \
    common.h \
    dialog.h

FORMS    += mainwindow.ui \
    login.ui \
    dialog.ui
