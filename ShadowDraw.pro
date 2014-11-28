#-------------------------------------------------
#
# Project created by QtCreator 2014-09-06T17:20:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PhotoManipulate
TEMPLATE = app

LIBS += -lopencv_core -lopencv_imgproc -lopencv_highgui


SOURCES += main.cpp\
        mainwindow.cpp \
    histogramdialog.cpp \
    convolutedialog.cpp \
    resizedialog.cpp

HEADERS  += mainwindow.h \
    histogramdialog.h \
    convolutedialog.h \
    resizedialog.h

FORMS    += mainwindow.ui \
    histogramdialog.ui \
    convolutedialog.ui \
    resizedialog.ui
