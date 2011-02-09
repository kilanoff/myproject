#-------------------------------------------------
#
# Project created by QtCreator 2011-01-25T15:50:25
#
#-------------------------------------------------


TARGET = pic2net
TEMPLATE = app

CONFIG += qxt
QXT += core gui

QT      += core gui network

#input
SOURCES += main.cpp mainwindow.cpp screenshot.cpp screenshotframe.cpp
HEADERS  += mainwindow.h screenshot.h screenshotframe.h
FORMS    += mainwindow.ui
RESOURCES += images.qrc
