#####################################################################
# Author:                                                           #
#	Edwin Christian Yllanes Cucho <e.yllanescucho@ieee.org>     #
#				      <e.yllanescucho@gmail.com>    #
#####################################################################
QT       += core gui
CONFIG  += qxt
QXT     += core gui

TARGET = MS

TEMPLATE = app

VERSION = 1.00

DEFINES += APP_VERSION=$$VERSION

FORMS = CameraConnectDialog.ui MainWindow.ui ProcessingSettingsDialog.ui \
    settingsdialog.ui

SOURCES += main.cpp\
    MainWindow.cpp \
    CaptureThread.cpp \
    Controller.cpp \
    ImageBuffer.cpp \
    CameraConnectDialog.cpp \
    ProcessingThread.cpp \
    FrameLabel.cpp \
    ProcessingSettingsDialog.cpp \
    MatToQImage.cpp \
    settingsdialog.cpp \
    SendThread.cpp \
    qledindicator.cpp

HEADERS  += MainWindow.h \
    CaptureThread.h \
    Controller.h \
    ImageBuffer.h \
    CameraConnectDialog.h \
    ProcessingThread.h \
    FrameLabel.h \
    ProcessingSettingsDialog.h \
    Structures.h \
    Config.h \
    MatToQImage.h \
    settingsdialog.h \
    SendThread.h \
    qledindicator.h

unix: CONFIG += link_pkgconfig \
    serialport
unix: PKGCONFIG += opencv \
#    cvblob

OTHER_FILES += \
    README.md

RESOURCES += \
    MyResource.qrc
