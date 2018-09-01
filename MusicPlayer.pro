#-------------------------------------------------
#
# Project created by QtCreator 2018-07-28T11:45:28
#
#-------------------------------------------------

QT       += core gui xml multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MusicPlayer
TEMPLATE = app
RC_ICONS = app.ico

CONFIG += c++11
MOC_DIR += $$PWD/temp
OBJECTS_DIR += $$PWD/temp
DESTDIR += $$PWD/bin

SOURCES += \
    core/main.cpp \
    UIBase/UIBaseWindow.cpp \
    UIBase/UIGlobalConfig.cpp \
    UIBase/UIGlobalTool.cpp \
    UIBase/UIMessageBox.cpp \
    UIBase/UIProgressWindow.cpp \
    MusicUI/MusicWidget.cpp \
    MusicUI/AudioPlayThread.cpp \
    MusicUI/MusicDecodecThread.cpp \
    MusicUI/MusicListWidget.cpp \
    MusicUI/MusicBottomControl.cpp

HEADERS  += \
    UIBase/UIBaseWindow.h \
    UIBase/UIGlobalConfig.h \
    UIBase/UIGlobalTool.h \
    UIBase/UIMessageBox.h \
    UIBase/UIProgressWindow.h \
    MusicUI/MusicWidget.h \
    MusicUI/AudioPlayThread.h \
    MusicUI/MusicDecodecThread.h \
    MusicUI/MusicLisWidget.h \
    MusicUI/MusicBottomControl.h

INCLUDEPATH += $$PWD/include
LIBS += -L $$PWD/lib -lavformat -lavcodec -lavutil -lswscale -lswresample -lavfilter
