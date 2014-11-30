#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:20:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SeamCarving
TEMPLATE = app


SOURCES += main.cpp\
            mainwindow.cpp \
            drawwidget.cpp \
            seamcarving.cpp \
    energydistview.cpp \
    gradientenergy.cpp

HEADERS  += mainwindow.h \
            drawwidget.h \
            seamcarving.h \
    energydistview.h \
    energyfunctioni.h \
    gradientenergy.h

FORMS    += mainwindow.ui \
    energydistview.ui

QMAKE_CXXFLAGS_RELEASE += -O3
