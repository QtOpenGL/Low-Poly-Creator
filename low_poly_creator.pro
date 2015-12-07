#-------------------------------------------------
#
# Project created by QtCreator 2015-12-06T21:30:39
#
#-------------------------------------------------

QT += core gui
QT += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = poly_creator_editor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    currentscene.cpp \
    Objects/ModelMesh.cpp \
    renderstate.cpp

HEADERS  += mainwindow.h \
    currentscene.h \
    Objects/ModelMesh.h \
    renderstate.h

FORMS    += mainwindow.ui

RESOURCES += \
    content.qrc
