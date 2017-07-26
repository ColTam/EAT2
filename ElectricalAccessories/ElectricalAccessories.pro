#-------------------------------------------------
#
# Project created by QtCreator 2017-04-27T16:52:52
#
#-------------------------------------------------

QT       += core gui axcontainer network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia printsupport

TARGET = ElectricalAccessories
TEMPLATE = app
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        widget.cpp \
    Collect.cpp \
    countdown.cpp \
    formdisplay.cpp \
    lifetesterdialog.cpp \
    minidatewidget.cpp \
    mloadthread.cpp \
    msplashscreen.cpp \
    mtestthread.cpp \
    mtrcrthread.cpp \
    pathdialog.cpp \
    pieceslist.cpp \
    qcustomplot.cpp \
    qmessagewidget.cpp \
    registrationcode.cpp \
    temperaturerisedialog.cpp \
    titlebar.cpp \
    helpWidget/mHelper.cpp \
    zqwl.cpp \
    mupdatethread.cpp

HEADERS  += widget.h \
    Collect.h \
    countdown.h \
    formdisplay.h \
    lifetesterdialog.h \
    minidatewidget.h \
    mloadthread.h \
    msplashscreen.h \
    mtestthread.h \
    mtrcrthread.h \
    pathdialog.h \
    pieceslist.h \
    qcustomplot.h \
    qmessagewidget.h \
    registrationcode.h \
    temperaturerisedialog.h \
    titlebar.h \
    nofocusdelegate.h \
    helpWidget/mHelper.h \
    zqwl.h \
    mupdatethread.h

FORMS    += \
    formdisplay.ui \
    lifetesterdialog.ui \
    temperaturerisedialog.ui

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/./ -lCollectCard
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/./ -lCollectCard
else:unix: LIBS += -L$$PWD/./ -lCollectCar

INCLUDEPATH += $$PWD/.
DEPENDPATH += $$PWD/.

TRANSLATIONS += zh_CN.ts

CONFIG(release, debug|release): QMAKE_LFLAGS += /MANIFESTUAC:\"level=\'requireAdministrator\' uiAccess=\'false\'\"

RC_ICONS = EATA.ico

RESOURCES += \
    resource.qrc
