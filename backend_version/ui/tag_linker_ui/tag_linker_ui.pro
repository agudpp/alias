#-------------------------------------------------
#
# Project created by QtCreator 2017-05-28T11:52:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

TARGET = tag_linker_ui
TEMPLATE = app


INCLUDEPATH += rapidjson

SOURCES += main.cpp\
        mainwindow.cpp \
    beconn.cpp

HEADERS  += mainwindow.h \
    beconn.h

FORMS    += mainwindow.ui
