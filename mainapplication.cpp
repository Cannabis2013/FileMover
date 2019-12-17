#include "mainapplication.h"

MainApplication::MainApplication(const QString &appName,
                                 const QString &orgName)
{
    rManager = new rulesManager(appName,orgName);
    sManager = new settingsManager(appName,orgName);
    entityManager = new EntityQueueManager();
    fManager = new FileInformationManager(appName,orgName);
    fWorker = new FileOperationWorker();
    fWatcher = new FileSystemWatcher(sManager->paths());
    fileWorkerThread = new QThread();

    fWorker->moveToThread(fileWorkerThread);

    QString ressourceFolderPath = "Ressources";

    qRegisterMetaType<DirectoryItem>("DirectoryItem");
    qRegisterMetaType<QList<DirectoryItem>>("QList<DirectoryItem>");

    qRegisterMetaType<EntityModel>("EntityModel");
    qRegisterMetaType<DirectoryCountEntity>("DirectoryCountEntity");

    /*
     *  Connnections
     */

    // Entity queue related..
    connect(fWorker,&fW::requestNextEntity,entityManager,&EntityQueueManager::sendNextEntity);
    connect(entityManager,&EntityQueueManager::sendEntity,fWorker,&fW::processEntity);

    // Detailed directory information..
    connect(sManager,&settingsManager::processPath,entityManager,&EntityQueueManager::addEntity);
    connect(fWatcher,&FileSystemWatcher::folderChanged,entityManager,&EntityQueueManager::addEntity);
    connect(fWatcher,&FileSystemWatcher::sendSystemTrayMessage,this,&MainApplication::sendSystemTrayMessage);
    connect(sManager,&settingsManager::removeItem,fManager,&FileInformationManager::removeItem);

    connect(fWorker,&fW::processFinished,fManager,&FileInformationManager::insertItems);
    connect(fWorker,&FileOperationWorker::sendFolderSizeEntity,this,&MainApplication::sendFolderSize);
    connect(fWorker,&FileOperationWorker::sendStatusLineMessage,this,&MainApplication::sendStatusMessage);

    // Notify observers related..
    connect(sManager,&settingsManager::stateChanged,this,&MainApplication::stateChanged);
    connect(rManager,&rulesManager::stateChanged,this,&MainApplication::stateChanged);
    connect(fManager,&FileInformationManager::stateChanged,this,&MainApplication::stateChanged);

    connect(entityManager,&EntityQueueManager::wakeUpProcess,fWorker,&FileOperationWorker::handleProcessRequest);

    connect(fWorker,&fW::jobDone,this,&ICoreApplication::stateChanged);

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
    FileObjectList allFiles = fW::generateFileObjects(paths);
    FileActionEntity *entity = new FileActionEntity();
    entity->setDirectoryPaths(paths);
    entity->setDirectoryFileContent(allFiles);
    entityManager->addEntity(entity);
}

void MainApplication::clearFoldersAccordingToRules(QStringList paths)
{
    const QList<Rule>rules = rManager->ruleslist();
    if(rules.isEmpty())
        clearFolders(paths);
    for(Rule r : rules)
    {
        FileObjectList allFiles = fW::generateFileObjects(paths,r.appliesToPath,r.typeFilter);
        for(SubRule sR : r.subRules)
            allFiles = fWorker->processFileObjects(allFiles,sR);

        FileActionEntity *entity = new FileActionEntity;
        entity->setDirectoryPaths(paths);
        entity->setFileActionDestinations(r.destinationPath);
        entity->setDirectoryFileContent(allFiles);
        entity->setFileActionRule(r.actionRuleEntity);
        entityManager->addEntity(entity);
    }
}

void MainApplication::addWatchFolders(QStringList paths)
{
    sManager->insertPath(paths);
}

void MainApplication::clearWatchFolders()
{
    sManager->clearPaths();

}

QString MainApplication::watchFolder(int index) const
{
    return sManager->paths().value(index);
}

QStringList MainApplication::watchFolders()
{
    return sManager->paths();
}

void MainApplication::clearRules() const
{
    for (int i = 0; i < rManager->ruleCount(); ++i)
        rManager->removeRuleAt(i);
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
