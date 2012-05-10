#-------------------------------------------------
#
# Project created by QtCreator 2012-01-21T20:30:21
#
#-------------------------------------------------

QT       += core gui
QT       += opengl
QT       += network

TARGET = AirBattle
TEMPLATE = app

SOURCES += main.cpp\
        airbattle.cpp \
    View.cpp \
    GameCenter.cpp \
    Element.cpp \
    Record.cpp \
    filecenter.cpp \
    server.cpp \
    network.cpp \
    humanconsole.cpp \
    aiBoss.cpp

HEADERS  += airbattle.h \
    View.h \
    GameCenter.h \
    Element.h \
    const.h \
    Record.h \
    filecenter.h \
    server.h \
    network.h \
    humanconsole.h \
    common.h \
    aiBoss.h

FORMS    += airbattle.ui





















