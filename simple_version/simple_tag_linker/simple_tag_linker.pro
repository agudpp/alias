#-------------------------------------------------
#
# Project created by QtCreator 2016-08-29T22:06:18
#
#-------------------------------------------------

QT       += core gui network
CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = simple_tag_linker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    connector.cpp

HEADERS  += mainwindow.h \
    connector.h

FORMS    += mainwindow.ui
