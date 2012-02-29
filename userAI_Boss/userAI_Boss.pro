CONFIG += console
CONFIG -= app_bundle

SOURCES += \
    main.cpp \
    gameclient.cpp \
    network.cpp \
    clientreceiverthread.cpp

HEADERS += \
    gameclient.h \
    common.h \
    utils.h \
    network.h \
    const.h \
    clientreceiverthread.h

QT += network


