QT       += core gui

QMAKE_LFLAGS += -lX11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtmpv
TEMPLATE = app

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv

SOURCES += src/qtmpv.cpp \
    src/main.cpp \
    src/overlay/overlaytext.cpp \
    src/overlay/overlayline.cpp
HEADERS += \
    src/overlay/overlayline.hpp \
    src/overlay/overlaytext.hpp \
    src/qtmpv.hpp
