HEADERS += \
    common/common.h \
    common/const.h \
    common/utils.h \
    server/gameserver.h \
    common/network.h \
    server/serverreceiverthread.h \
    server/serversenderthread.h

SOURCES += \
    common/common.cpp \
    common/utils.cpp \
    server/gameserver.cpp \
    common/network.cpp \
    server/main.cpp \
    server/serverreceiverthread.cpp \
    server/serversenderthread.cpp

QT += network
