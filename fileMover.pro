#-------------------------------------------------
#
# Project created by QtCreator 2014-12-01T23:00:13
#
#-------------------------------------------------

QT       += core gui testlib

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
CONFIG   += C++17
}


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Filehandler
TEMPLATE = app


RC_ICONS = xIcon.ico

SOURCES += main.cpp\
    abstractruledialog.cpp \
    abstractframeimplementable.cpp \
    abstractpersistence.cpp \
    abstracttextselectordialog.cpp \
    conditionwidget.cpp \
    customdialog.cpp \
    customsplitter.cpp \
    customsplitterhandle.cpp \
    fileinformationmanager.cpp \
    fileinformationview.cpp \
    filemodel.cpp \
    filemodeldelegate.cpp \
    fileoperationworker.cpp \
    filepathdialogwidget.cpp \
    filesystemwatcher.cpp \
    icoreapplication.cpp \
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
    testfilecreator.cpp \
    tst_core_functionality.cpp \
    worker.cpp \
    topframeform.cpp

HEADERS  += mainwindow.h \
    NotifyDomain.h \
    abstractruledialog.h \
    abstractframeimplementable.h \
    abstractpersistence.h \
    abstracttextselectordialog.h \
    conditionwidget.h \
    customcombobox.h \
    customdialog.h \
    customsplitter.h \
    customsplitterhandle.h \
    entitymodel.h \
    entityqueuemanager.h \
    fileinformationmanager.h \
    filemodel.h \
    filemodeldelegate.h \
    fileoperationworker.h \
    filepathdialogwidget.h \
    filesystemwatcher.h \
    icoreapplication.h \
    mainapplication.h \
    messageBox.h \
    myicon.h \
    myicon.h \
    myicon.h \
    rulesmanager.h \
    myscreendimensions.h \
    mydatetime.h \
    settings_structure.h \
    settingsmanager.h \
    settingswindow.h \
    fileinformationview.h \
    addfolderwidget.h \
    addfiledialog.h \
    pathline.h \
    rulepathselector.h \
    addruledialog.h \
    editruledialog.h \
    ruledefinitions.h \
    rules.h \
    testfilecreator.h \
    worker.h \
    topframeform.h

FORMS    += mainwindow.ui \
    conditionwidget.ui \
    customdialog.ui \
    logger.ui \
    settingswindow.ui \
    textpathedit.ui \
    addfolderwidget.ui \
    abstractruledialog.ui \
    filepathselectordialog.ui \
    topframeform.ui

DISTFILES += \
    tempBuffer

RESOURCES += \
    myressource.qrc
