TEMPLATE = app

QT += qml quick widgets

SOURCES += main.cpp \
    core/elements/ElementsHandler.cpp

RESOURCES += qml.qrc
QMAKE_CXXFLAGS += -std=c++11



# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    core/types/basic_types.h \
    core/elements/Element.h \
    core/elements/tag/Tag.h \
    core/elements/link/Link.h \
    core/elements/ElementHolder.h \
    core/debug/DebugUtil.h \
    core/elements/content/ContentElement.h \
    core/elements/ElementsHandler.h
