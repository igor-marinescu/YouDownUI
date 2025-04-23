#-------------------------------------------------
#
# Project created by QtCreator 2019-12-05T11:17:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

win32:RC_FILE = YouDownUI.rc

TARGET = YouDownUI
TEMPLATE = app

#DEFINES += DEBUGON

# Add application name and version to defines
DEFINES +="APP_MAME=\\\"YouDownUI\\\""
DEFINES +="APP_VERS=\\\"1.4\\\""

SOURCES += main.cpp\
    mainwindow.cpp \
    settings.cpp \
    ConfFile.cpp \
    AppInterface.cpp \
    logging.cpp \
    queue.cpp \
    add_edit.cpp \
    editlinetext.cpp \
    yprocess.cpp \
    utils.cpp

HEADERS  += mainwindow.h \
    settings.h \
    ConfFile.h \
    AppInterface.h \
    logging.h \
    queue.h \
    add_edit.h \
    editlinetext.h \
    yprocess.h \
    utils.h

FORMS    += mainwindow.ui \
    settings.ui \
    add_edit.ui \
    editlinetext.ui

OTHER_FILES += \
    YouDownUI.rc
