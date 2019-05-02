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

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Filehandler
TEMPLATE = app

RC_ICONS = xIcon.ico

SOURCES += main.cpp\
    AbstractRuleDialog.cpp \
    abstracgraphicallistmanager.cpp \
    abstractcoreapplication.cpp \
    abstractpersistence.cpp \
    fileinformationmanager.cpp \
    fileinformationview.cpp \
    fileworker.cpp \
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
    mysplitterhandle.cpp \
    myverticalsplitter.cpp \
    myhorizontalsplitter.cpp \
    rulepathselector.cpp \
    rulepathdialogselector.cpp \
    addruledialog.cpp \
    editruledialog.cpp \
    conditionWidget.cpp \
    worker.cpp \
    filepathselectordialog.cpp \
    mywidget.cpp \
    widgetform.cpp \
    topframeform.cpp

HEADERS  += mainwindow.h \
    AbstractRuleDialog.h \
    MyIcon.h \
    abstractcoreapplication.h \
    abstractpersistence.h \
    customcombobox.h \
    fileinformationmanager.h \
    fileworker.h \
    iconscanner.h \
    mainapplication.h \
    messageBox.h \
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
    mysplitterhandle.h \
    myverticalsplitter.h \
    myhorizontalsplitter.h \
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
    widgetform.h \
    topframeform.h

FORMS    += mainwindow.ui \
    logger.ui \
    settingsWindow.ui \
    textpathedit.ui \
    addfolderwidget.ui \
    abstractRuledialog.ui \
    conditionWidget.ui \
    filepathselectordialog.ui \
    widgetform.ui \
    topframeform.ui

DISTFILES += \
    tempBuffer

RESOURCES += \
    myressource.qrc
