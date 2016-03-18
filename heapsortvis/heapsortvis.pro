#-------------------------------------------------
#
# Project created by QtCreator 2016-03-05T17:22:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = heapsortvis
TEMPLATE = app


INCLUDEPATH += ./include
DEPENDPATH  += ./include

INCLUDEPATH += ./src_hs
DEPENDPATH  += ./src_hs

INCLUDEPATH += ./src_ghs
DEPENDPATH  += ./src_ghs


SOURCES += \
    src/main.cpp \
    src/wmainwindow.cpp \
    src/datainputvalidator.cpp \
    src/visutil.cpp \
    src/heapifyanimation.cpp \
    src/animationbase.cpp \
    src/sortanimation.cpp

HEADERS  += \
    include/wmainwindow.h \
    include/datainputvalidator.h \
    include/visutil.h \
    include/heapifyanimation.h \
    include/animationbase.h \
    include/sortanimation.h

FORMS    += \
    forms/wmainwindow.ui
