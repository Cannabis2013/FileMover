#include "applicationui.h"
#include <qapplication.h>
#include <iostream>

#include "fileworker.h"
#include "threadsmanager.h"
#include "settingsmanager.h"
#include "applicationdomain.h"
#include "filesystemwatcher.h"
#include "fileinformationmanager.h"
#include "entityqueuemanager.h"
#include "filelistservice.h"
#include "rulebuilder.h"
#include "rulesmanager.h"
#include "ruledefinitions.h"

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

        AbstractApplicationService *mainApplicaton = new ApplicationDomain();

        mainApplicaton->setFileOperationsService(new FileWorker());
        mainApplicaton->setRuleManagerService(new rulesManager(argVals.appName,argVals.orgName,new RuleBuilder()));
        mainApplicaton->setThreadManagerService(new ThreadsManager());
        mainApplicaton->setSettingsManagerService(new settingsManager(argVals.appName,argVals.orgName));
        mainApplicaton->setFileInformationManagerService(new FileInformationManager(argVals.appName,argVals.orgName));
        mainApplicaton->setEntityQueueManagerService(new EntityQueueManager());
        mainApplicaton->setFileWatcherService(new FileSystemWatcher());
        mainApplicaton->setFileModelBuilderService(new FileListService());
        mainApplicaton->setRuleDefinitionsService(new RuleDefinitions());
        mainApplicaton->configureServices();
        mainApplicaton->startServices();


        QMainWindow *w = new ApplicationUI(mainApplicaton,new RuleBuilder, new RuleDefinitions);

        w->show();

        return a.exec();
    }
#endif
