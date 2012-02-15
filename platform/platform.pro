CONFIG += console
CONFIG -= app_bundle

HEADERS += \
    common/common.h \
    common/const.h \
    common/utils.h \
    server/gameserver.h \
    common/network.h \
    server/serverreceiverthread.h

SOURCES += \
    common/common.cpp \
    common/utils.cpp \
    server/gameserver.cpp \
    common/network.cpp \
    server/main.cpp \
    server/serverreceiverthread.cpp

QT += network

