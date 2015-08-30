#-------------------------------------------------
#
# Project created by QtCreator 2015-05-23T20:02:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TimeTable
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    delegate.cpp \
    graphelem.cpp \
    edge.cpp \
    graphelemid.cpp \
    tableeditform.cpp \
    tableinsertform.cpp \
    gantform.cpp

HEADERS  += mainwindow.h \
    delegate.h \
    graphelem.h \
    edge.h \
    graphelemid.h \
    tableeditform.h \
    tableinsertform.h \
    gantform.h

FORMS    += mainwindow.ui \
    tableeditform.ui \
    tableinsertform.ui \
    gantform.ui
