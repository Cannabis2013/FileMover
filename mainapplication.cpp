#include "mainapplication.h"

MainApplication::MainApplication(QString appName, QString orgName)
{
    rManager = new rulesManager(appName,orgName);
    sManager = new settingsManager(appName,orgName);
    pManager = new ProcessManager();
    fManager = new FileInformationManager(appName,orgName);
    fWorker = new FileWorker(pManager);
    fileWorkerThread = new QThread();

    fWorker->moveToThread(fileWorkerThread);

    QString ressourceFolderPath = "Ressources";

    qRegisterMetaType<DirectoryItem>("DirectoryItem");
    qRegisterMetaType<QList<DirectoryItem>>("QList<DirectoryItem>");
    qRegisterMetaType<DirectoryObject>("DirectoryObject");
    qRegisterMetaType<QList<DirectoryObject>>("QList<DirectoryObject>");

    // Detailed directory information
    connect(sManager,&settingsManager::processPath,fWorker,&FileWorker::processFileInformation);
    connect(sManager,&settingsManager::processPaths,fWorker,&FileWorker::processFileInformations);

    connect(sManager,&settingsManager::removeItem,fManager,&FileInformationManager::removeItem);

    connect(fWorker,&fW::processFinished,fManager,&FileInformationManager::insertItem);
    connect(fWorker,&fW::multipleProcessFinished,fManager,&FileInformationManager::insertItems);

    connect(fWorker,&FileWorker::sendFolderSizeEntity,this,&MainApplication::sendFolderSize);
    connect(fWorker,&FileWorker::sendFolderSizeEntities,this,&MainApplication::sendFolderSizes);

    connect(fWorker,&FileWorker::itemText,this,&MainApplication::sendFilePath);

    connect(sManager,&settingsManager::stateChanged,this,&MainApplication::stateChanged);
    connect(rManager,&rulesManager::stateChanged,this,&MainApplication::stateChanged);
    connect(fManager,&FileInformationManager::stateChanged,this,&MainApplication::stateChanged);

    connect(pManager,&ProcessManager::wakeUpProcess,fWorker,&FileWorker::handleProcessRequest);

    connect(fWorker,&fW::jobDone,this,&AbstractCoreApplication::stateChanged);

    fileWorkerThread->start();

    emit sManager->processPaths(watchFolders());
}

MainApplication::~MainApplication()
{
    delete sManager;
    delete fManager;
    delete rManager;
}

void MainApplication::clearFolders(QStringList paths)
{
    QFileInfoList allFiles = fW::generateFilesList(QString(),paths,false);
    ProcessEntity entity;
    entity.directoryPaths = paths;
    entity.list = allFiles;
    pManager->addToQueue(entity);

}

void MainApplication::clearFoldersAccordingToRules(QStringList paths)
{
    QList<Rule>rules = rManager->ruleslist();
    for(Rule r : rules)
    {
        QFileInfoList allFiles = fW::generateFilesList(r.appliesToPath,paths,r.deepScanMode);
        for(SubRule sR : r.subRules)
            allFiles = fWorker->processList(allFiles,sR);

        // Implementer fileworker operation..

        ProcessEntity entity;
        entity.directoryPaths = paths;
        entity.destinations = r.destinationPath;
        entity.list = allFiles;
        entity.ruleMode = r.actionRule;
        pManager->addToQueue(entity);
    }
}

void MainApplication::addWatchFolders(QStringList paths)
{
    sManager->insertPaths(paths);
}


QString MainApplication::watchFolder(int index) const
{
    return sManager->paths().value(index);
}

QStringList MainApplication::watchFolders()
{
    return sManager->paths();
}

SettingsDelegate MainApplication::settingsState()
{
    return sManager->settingsState();
}

void MainApplication::setSettings(SettingsDelegate s)
{
    sManager->setSettings(s);
}

void MainApplication::removeWatchFolderAt(int index)
{
    sManager->removePathAt(index);
}

void MainApplication::removeWatchFolder(QString path)
{
    sManager->removePath(path);
}
