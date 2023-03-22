TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG += qt

DEFINES += ZMQ_STATIC
LIBS += -L$$PWD/../lib -lzmq -lnzmqt -lws2_32 -lIphlpapi
INCLUDEPATH += $$PWD/../include

SOURCES += main.cpp \
    diceroller.cpp

HEADERS += \
    diceroller.h
