#include "mainwindow.h"
#include <qapplication.h>
#include <iostream>


using namespace std;

#ifndef TEST_MODE
    struct argumentValues
    {
        QString orgName = "MH";
        QString appName = "Filehandler";
    };

    void processArguments(const QStringList &arguments,argumentValues &argVals)
    {
        for (int i = 0;i <arguments.count();i++) {
            QString argument = arguments.value(i);
            if(argument == "--application-name")
                argVals.appName = arguments.value(++i);
            if(argument == "--organization-name")
                argVals.appName = arguments.value(++i);
        }
    }

    int main(int argc, char *argv[])
    {
        QApplication a(argc, argv);
        a.setQuitOnLastWindowClosed(true);

        argumentValues argVals;
        QStringList arguments = QApplication::arguments();

        processArguments(arguments,argVals);

        AbstractCoreApplication *mainApplicaton = new MainApplication();

        mainApplicaton->setFileOperationsService(new FileOperationWorker());
        mainApplicaton->setRuleManagerService(new rulesManager(argVals.appName,argVals.orgName));
        mainApplicaton->setThreadManagerService(new ThreadsManager());
        mainApplicaton->setSettingsManagerService(new settingsManager(argVals.appName,argVals.orgName));
        mainApplicaton->setFileInformationManagerService(new FileInformationManager(argVals.appName,argVals.orgName));
        mainApplicaton->setEntityQueueManagerService(new EntityQueueManager());
        mainApplicaton->setFileWatcherService(new FileSystemWatcher());

        mainApplicaton->configureServices();
        mainApplicaton->startServices();


        QMainWindow *w = new mainWindow(mainApplicaton);
        w->show();

        return a.exec();
    }
#endif
