HEADERS += \
    common/common.h \
    common/const.h \
    common/utils.h \
    server/gameserver.h \
    common/network.h \
    server/serverreceiverthread.h \
    server/serversenderthread.h \
    server/servertimer.h

SOURCES += \
    common/common.cpp \
    common/utils.cpp \
    server/gameserver.cpp \
    common/network.cpp \
    server/main.cpp \
    server/serverreceiverthread.cpp \
    server/serversenderthread.cpp \
    server/servertimer.cpp

QT += network
