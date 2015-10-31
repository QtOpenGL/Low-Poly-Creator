#-------------------------------------------------
#
# Project created by QtCreator 2015-07-06T19:21:29
#
#-------------------------------------------------

QT += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Virtual_Creator
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    renderstate.cpp \
    Objects/ModelMesh.cpp \
    Objects/Node.cpp

HEADERS  += mainwindow.h \
    renderstate.h \
    Objects/ModelMesh.h \
    Objects/Node.h

FORMS    += mainwindow.ui

RESOURCES += \
    content.qrc
