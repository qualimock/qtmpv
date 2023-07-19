QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtmpv
TEMPLATE = app

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv

SOURCES += src/qtmpv.cpp \
    src/main.cpp \
    src/overlay/overlaytext.cpp \
    src/overlay/overlayline.cpp \
    src/overlay/overlaywidget.cpp
HEADERS += src/qtmpv.h \
    src/overlay/overlaytext.h \
    src/overlay/overlayline.h \
    src/overlay/overlaywidget.h
