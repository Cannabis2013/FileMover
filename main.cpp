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
#include "settingswindowbuilder.h"
#include "filteringcontext.h"
#include "filelistservice.h"
#include "filteringcontext.h"
#include "filesystemwatcher.h"
#include "settingsbuilder.h"
#include "entitymodelbuilder.h"

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

        auto fileListService = (new FileListService())->setModelBuilderService(new FileModelBuilder());

        auto settingsBuilder = new SettingsBuilder();
        auto entityBuilder = new EntityModelBuilder();

        auto mainApplicaton = (new ApplicationDomain())->
                setFileOperationsService(new FileWorker())->
                setRuleManagerService(new rulesManager(argVals.appName,argVals.orgName,new RuleBuilder()))->
                setThreadManagerService(new ThreadsManager())->
                setSettingsManagerService(new settingsManager(argVals.appName,argVals.orgName,settingsBuilder,entityBuilder))->
                setFileInformationManagerService(new FileInformationManager(argVals.appName,argVals.orgName))->
                setEntityQueueManagerService(new EntityQueueManager())->
                setFileWatcherService(new FileSystemWatcher(entityBuilder))->
                setFileModelBuilderService(fileListService)->
                setRuleDefinitionsService(new RuleDefinitions())->
                setFilteringContext(new FilteringContext(entityBuilder),new FileListService())->
                setSettingsBuilderService(settingsBuilder)->
                setEntityModelBuilderService(entityBuilder)->
                configureServices()->
                startServices();

        auto w = new ApplicationUI(mainApplicaton,new RuleBuilder, new RuleDefinitions);

        w->setSettingsBuilderService(new SettingsWindowBuilder());

        w->show();

        return a.exec();
    }
#endif
