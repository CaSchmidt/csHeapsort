#-------------------------------------------------
#
# Project created by QtCreator 2016-03-18T16:54:12
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = algorithm
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


INCLUDEPATH += ./include
DEPENDPATH  += ./include


SOURCES += \
    src/heapsort.c \
    src/sort.c \
    src/main.cpp \
    src/insertionsort.c

HEADERS += \
    include/sort.h
