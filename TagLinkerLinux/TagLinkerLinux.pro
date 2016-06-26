#-------------------------------------------------
#
# Project created by QtCreator 2016-04-11T20:39:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TagLinkerLinux
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        MainWindow.cpp \
    content/IContent.cpp \
    content/ContentHandler.cpp \
    content/Tag.cpp \
    content/TagDBHandler.cpp \
    content/TagContentMapper.cpp \
    content/ContentManager.cpp \
    completer/ICompleter.cpp \
    completer/CompleterManager.cpp

HEADERS  += MainWindow.h \
    core/debug/Debug.h \
    content/IContent.h \
    content/ContentHandler.h \
    content/Tag.h \
    content/TagDefs.h \
    content/TagDBHandler.h \
    content/TagContentMapper.h \
    content/IContentDefs.h \
    content/ContentManager.h \
    completer/ICompleter.h \
    completer/CompleterManager.h \
    completer/ICompleterDefs.h

FORMS    += mainwindow.ui
