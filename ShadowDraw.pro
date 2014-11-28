#-------------------------------------------------
#
# Project created by QtCreator 2014-11-27T00:54:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ShadowDraw
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    drawwidget.cpp \
    seamcarving.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    seamcarving.h

FORMS    += mainwindow.ui
