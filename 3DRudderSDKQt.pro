#-------------------------------------------------
#
# Project created by QtCreator 2016-08-11T09:49:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = 3DRudderSDKQt
TEMPLATE = app


SOURCES += main.cpp\
        maindialog.cpp

HEADERS  += maindialog.h \
    ../3DRudderSDK/Include/3DRudderSDK.h

FORMS    += maindialog.ui

INCLUDEPATH += $$PWD/../3DRudderSDK/Include/
DEPENDPATH += $$PWD/../3DRudderSDK/Include/

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../3DRudderSDK/Lib/x64/Static/ -l3DRudderSDK
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../3DRudderSDK/Lib/x64/Static/ -l3DRudderSDK

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3DRudderSDK/Lib/x64/Static/lib3DRudderSDK.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3DRudderSDK/Lib/x64/Static/lib3DRudderSDK.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../3DRudderSDK/Lib/x64/Static/3DRudderSDK.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../3DRudderSDK/Lib/x64/Static/3DRudderSDK.lib
