HEADERS += \
    common/common.h \
    common/const.h \
    common/utils.h \
    client/gameclient.h \
    server/gameserver.h \
    display/gamedisplay.h \
    client/receiverthread.h

SOURCES += \
    common/common.cpp \
    common/utils.cpp \
    client/gameclient.cpp \
    server/gameserver.cpp \
    display/gamedisplay.cpp \
    client/receiverthread.cpp \
    client/main.cpp

QT += network
