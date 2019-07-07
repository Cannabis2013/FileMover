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
    abstractcoreapplication.cpp \
    abstractframeimplementable.cpp \
    abstractpersistence.cpp \
    abstracttextselectordialog.cpp \
    customdialog.cpp \
    customsplitter.cpp \
    customsplitterhandle.cpp \
    entitymodel.cpp \
    fileinformationmanager.cpp \
    fileinformationview.cpp \
    fileobject.cpp \
    fileoperationworker.cpp \
    filepathdialogwidget.cpp \
    mainapplication.cpp \
        mainwindow.cpp \
    rulesmanager.cpp \
    mydatetime.cpp \
    settingsmanager.cpp \
    settingswindow.cpp \
    addfolderwidget.cpp \
    addfiledialog.cpp \
    pathline.cpp \
    rulepathselector.cpp \
    addruledialog.cpp \
    editruledialog.cpp \
    conditionWidget.cpp \
    worker.cpp \
    mywidget.cpp \
    topframeform.cpp

HEADERS  += mainwindow.h \
    AbstractRuleDialog.h \
    MyIcon.h \
    abstractcoreapplication.h \
    abstractframeimplementable.h \
    abstractpersistence.h \
    abstracttextselectordialog.h \
    customcombobox.h \
    customdialog.h \
    customsplitter.h \
    customsplitterhandle.h \
    entitymodel.h \
    entityqueuemanager.h \
    fileinformationmanager.h \
    fileobject.h \
    fileoperationworker.h \
    filepathdialogwidget.h \
    filesystemwatcher.h \
    mainapplication.h \
    messageBox.h \
    myIcon.h \
    myIcon.h \
    myobject.h \
    rulesmanager.h \
    myscreendimensions.h \
    mydatetime.h \
    settingsdelegate.h \
    settingsmanager.h \
    settingswindow.h \
    fileinformationview.h \
    addfolderwidget.h \
    addfiledialog.h \
    pathline.h \
    rulepathselector.h \
    addruledialog.h \
    editruledialog.h \
    conditionWidget.h \
    ruledefinitions.h \
    rules.h \
    worker.h \
    mywidget.h \
    topframeform.h

FORMS    += mainwindow.ui \
    customdialog.ui \
    logger.ui \
    settingswindow.ui \
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
