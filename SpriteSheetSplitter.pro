#-------------------------------------------------
#
# Project created by QtCreator 2018-08-05T14:25:02
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = SpriteSheetSplitter
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TRANSLATIONS = spriter_fr.ts

RC_FILE = myapp.rc

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    inputtab.cpp \
    grid.cpp

HEADERS += \
        mainwindow.h \
    inputtab.h \
    grid.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
