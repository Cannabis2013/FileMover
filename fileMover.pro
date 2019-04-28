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
    abstracgraphicallistmanager.cpp \
    abstractpersistence.cpp \
    fileinformationview.cpp \
    mainapplication.cpp \
        mainwindow.cpp \
    fileworker.cpp \
    myapp.cpp \
    rulesmanager.cpp \
    settingsWindow.cpp \
    fileinformation.cpp \
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
    abstractRuledialog.cpp \
    addruledialog.cpp \
    editruledialog.cpp \
    conditionWidget.cpp \
    worker.cpp \
    fileworkeroperator.cpp \
    filepathselectordialog.cpp \
    mywidget.cpp \
    widgetform.cpp \
    topframeform.cpp

HEADERS  += mainwindow.h \
    MyIcon.h \
    abstractpersistence.h \
    iconscanner.h \
    mainapplication.h \
    messageBox.h \
    fileworker.h \
    myapp.h \
    processmanager.h \
    rulesmanager.h \
    settingsWindow.h \
    myscreendimensions.h \
    fileinformation.h \
    mydatetime.h \
    mycombobox.h \
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
    abstractRuleDialog.h \
    addruledialog.h \
    editruledialog.h \
    conditionWidget.h \
    argumentvalues.h \
    ruledefinitions.h \
    rules.h \
    worker.h \
    fileworkeroperator.h \
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
