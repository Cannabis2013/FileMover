#include "mainapplication.h"

MainApplication::MainApplication(QString appName, QString orgName)
{
    rManager = new rulesManager(appName,orgName);
    sManager = new settingsManager(appName,orgName);
    entityManager = new EntityQueueManager();
    fManager = new FileInformationManager(appName,orgName);
    fWorker = new FileOperationsWorker(entityManager);
    fileSystemWatcher = new QFileSystemWatcher(sManager->paths());
    fileWorkerThread = new QThread();

    fWorker->moveToThread(fileWorkerThread);

    QString ressourceFolderPath = "Ressources";

    qRegisterMetaType<DirectoryItem>("DirectoryItem");
    qRegisterMetaType<QList<DirectoryItem>>("QList<DirectoryItem>");

    qRegisterMetaType<EntityModel>("EntityModel");
    qRegisterMetaType<DirectoryCountEntity>("DirectoryCountEntity");

    // Detailed directory information
    connect(sManager,&settingsManager::processPath,entityManager,&EntityQueueManager::addEntity);
    connect(sManager,&settingsManager::removeItem,fManager,&FileInformationManager::removeItem);

    connect(fWorker,&fW::processFinished,fManager,&FileInformationManager::insertItems);
    connect(fWorker,&FileOperationsWorker::sendFolderSizeEntity,this,&MainApplication::sendFolderSize);
    connect(fWorker,&FileOperationsWorker::itemText,this,&MainApplication::sendFilePath);

    // Observer/observable related
    connect(sManager,&settingsManager::stateChanged,this,&MainApplication::stateChanged);
    connect(rManager,&rulesManager::stateChanged,this,&MainApplication::stateChanged);
    connect(fManager,&FileInformationManager::stateChanged,this,&MainApplication::stateChanged);

    connect(entityManager,&EntityQueueManager::wakeUpProcess,fWorker,&FileOperationsWorker::handleProcessRequest);

    connect(fWorker,&fW::jobDone,this,&AbstractCoreApplication::stateChanged);

    fileWorkerThread->start();

    emit sManager->processPath(new fileInformationEntity(watchFolders()));
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
    FileActionEntity *entity = new FileActionEntity();
    entity->setDirectoryPaths(paths);
    entity->setDirectoryFileContent(allFiles);
    entityManager->addEntity(entity);
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

        FileActionEntity *entity = new FileActionEntity;
        entity->setDirectoryPaths(paths);
        entity->setFileActionDestinations(r.destinationPath);
        entity->setDirectoryFileContent(allFiles);
        entity->setFileActionRule(r.actionRule);
        entityManager->addEntity(entity);
    }
}

void MainApplication::addWatchFolders(QStringList paths)
{
    sManager->insertPath(paths);
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

void MainApplication::calculateFolderSize(QString path)
{
    QFileInfo fInfo(path);
    DirectoryCountEntity *fObject = new DirectoryCountEntity();

    fObject->setDirectoryPath(fInfo.absoluteFilePath());
    fObject->setDirectoryName(fInfo.fileName());

    entityManager->addEntity(fObject);
}

void MainApplication::removeWatchFolderAt(int index)
{
    sManager->removePathAt(index);
}

void MainApplication::removeWatchFolder(QString path)
{
    sManager->removePath(path);
}
