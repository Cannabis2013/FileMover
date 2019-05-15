#-------------------------------------------------
#
# Project created by QtCreator 2014-12-01T23:00:13
#
#-------------------------------------------------

QT       += core gui

windows {
    *-g++* {
        # MinGW
    }
    *-msvc* {
        # MSVC

QT       += webenginewidgets
    }
}

win32
{
QMAKE_CXXFLAGS += -std=c++17

}
macx
{
CONFIG   += C++11
}

QT += testlib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Filehandler
TEMPLATE = app


RC_ICONS = xIcon.ico

SOURCES += main.cpp\
    AbstractRuleDialog.cpp \
    abstracgraphicallistmanager.cpp \
    abstractcoreapplication.cpp \
    abstractpersistence.cpp \
    customdialog.cpp \
    customsplitter.cpp \
    customsplitterhandle.cpp \
    fileinformationmanager.cpp \
    fileinformationview.cpp \
    fileworker.cpp \
    frameimplementable.cpp \
    mainapplication.cpp \
        mainwindow.cpp \
    rulesmanager.cpp \
    settingsWindow.cpp \
    mydatetime.cpp \
    settingsmanager.cpp \
    textpathedit.cpp \
    addfolderwidget.cpp \
    addfiledialog.cpp \
    pathline.cpp \
    rulepathselector.cpp \
    rulepathdialogselector.cpp \
    addruledialog.cpp \
    editruledialog.cpp \
    conditionWidget.cpp \
    worker.cpp \
    filepathselectordialog.cpp \
    mywidget.cpp \
    topframeform.cpp

HEADERS  += mainwindow.h \
    AbstractRuleDialog.h \
    MyIcon.h \
    abstractcoreapplication.h \
    abstractpersistence.h \
    customcombobox.h \
    customdialog.h \
    customsplitter.h \
    customsplitterhandle.h \
    fileinformationmanager.h \
    fileworker.h \
    frameimplementable.h \
    iconscanner.h \
    mainapplication.h \
    messageBox.h \
    myIcon.h \
    myIcon.h \
    processmanager.h \
    rulesmanager.h \
    settingsWindow.h \
    myscreendimensions.h \
    mydatetime.h \
    settingsmanager.h \
    textpathedit.h \
    fileinformationview.h \
    addfolderwidget.h \
    addfiledialog.h \
    pathline.h \
    rulepathselector.h \
    rulepathdialogselector.h \
    addruledialog.h \
    editruledialog.h \
    conditionWidget.h \
    ruledefinitions.h \
    rules.h \
    worker.h \
    filepathselectordialog.h \
    mywidget.h \
    topframeform.h

FORMS    += mainwindow.ui \
    customdialog.ui \
    logger.ui \
    settingsWindow.ui \
    textpathedit.ui \
    addfolderwidget.ui \
    abstractRuledialog.ui \
    conditionWidget.ui \
    filepathselectordialog.ui \
    topframeform.ui

DISTFILES += \
    tempBuffer

RESOURCES += \
    myressource.qrc

TEST += \
