QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtmpv
TEMPLATE = app

QT_CONFIG -= no-pkg-config
CONFIG += link_pkgconfig debug
PKGCONFIG += mpv

SOURCES += qtmpv.cpp \
    main.cpp \
    overlayline.cpp \
    overlaywidget.cpp
HEADERS += qtmpv.h \
    overlayline.h \
    overlaywidget.h
