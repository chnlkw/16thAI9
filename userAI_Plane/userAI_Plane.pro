CONFIG += console
CONFIG -= app_bundle

HEADERS += \
    utils.h \
    network.h \
    gameclient.h \
    const.h \
    common.h \
    clientreceiverthread.h

SOURCES += \
    network.cpp \
    gameclient.cpp \
    main.cpp \
    clientreceiverthread.cpp

QT += network


