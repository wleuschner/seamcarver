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
<<<<<<< HEAD
    histogramdialog.cpp \
    convolutedialog.cpp \
    resizedialog.cpp

HEADERS  += mainwindow.h \
    histogramdialog.h \
    convolutedialog.h \
    resizedialog.h
=======
    drawwidget.cpp \
    seamcarving.cpp

HEADERS  += mainwindow.h \
    drawwidget.h \
    seamcarving.h
>>>>>>> 3b5e1bd802335e3dda75fe61104e820d0695b6ee

FORMS    += mainwindow.ui \
    histogramdialog.ui \
    convolutedialog.ui \
    resizedialog.ui
