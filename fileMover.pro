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
    abstractframe.cpp \
    abstractruledialog.cpp \
    abstracttextselectordialog.cpp \
    applicationdomain.cpp \
    conditionwidget.cpp \
    customdialog.cpp \
    customsplitter.cpp \
    customsplitterhandle.cpp \
    exceptionhandler.cpp \
    fileinformationmanager.cpp \
    fileinformationview.cpp \
    filepathdialogwidget.cpp \
    filesystemwatcher.cpp \
    fileworker.cpp \
        mainwindow.cpp \
    rulesmanager.cpp \
    mydatetime.cpp \
    settingsmanager.cpp \
    settingswindow.cpp \
    addfolderwidget.cpp \
    addfiledialog.cpp \
    rulepathselector.cpp \
    addruledialog.cpp \
    editruledialog.cpp \
    testfilecreator.cpp \
    threadsmanager.cpp \
    tst_core_functionality.cpp \
    topframeform.cpp

HEADERS  += mainwindow.h \
    AbstractFileWorker.h \
    StaticStringsCollection.h \
    TestRuleHelper.h \
    TypeDefinitions.h \
    abstractapplicationservice.h \
    abstractfileinformationmanager.h \
    abstractfileoperationworker.h \
    abstractfilesystemwatcher.h \
    abstractframe.h \
    abstracticon.h \
    abstractqueueManager.h \
    abstractruledialog.h \
    abstractpersistence.h \
    abstractsettingsmanager.h \
    abstracttextselectordialog.h \
    applicationdomain.h \
    conditionwidget.h \
    customcombobox.h \
    customdialog.h \
    customsplitter.h \
    customsplitterhandle.h \
    directoryitem.h \
    entitymodel.h \
    entityqueuemanager.h \
    exceptionhandler.h \
    fileinformationmanager.h \
    filemodeldelegate.h \
    filepathdialogwidget.h \
    filesystemwatcher.h \
    fileworker.h \
    genericserviceinjector.h \
    icon.h \
    imutableobject.h \
    inheritexceptiondelegate.h \
    iruledefinitions.h \
    isettingsdelegate.h \
    ithreadmanagerinterface.h \
    messageBox.h \
    modeldelegates.h \
    rulesmanager.h \
    myscreendimensions.h \
    mydatetime.h \
    settingsdelegate.h \
    settingsmanager.h \
    settingsmodel.h \
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
    staticfilehelperoperations.h \
    staticviewhelpers.h \
    testfilecreator.h \
    threaddescriptor.h \
    threadsmanager.h \
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
