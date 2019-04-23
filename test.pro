#-------------------------------------------------
#
# Project created by QtCreator 2019-04-03T13:30:32
#
#-------------------------------------------------

QT       += core
QT       += network
QT       -= gui
QT       += sql

TARGET = test
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    test_server.cpp \
    http_helper.cpp \
    tinydb.cpp

HEADERS += \
    test_server.h \
    http_helper.h \
    tinydb.h
