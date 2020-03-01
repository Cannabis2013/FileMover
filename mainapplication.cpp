#include "mainapplication.h"

MainApplication::MainApplication()
{
}

MainApplication::~MainApplication()
{
    delete settingsManagerService;
    delete fileManagerService;
    delete ruleManagerService;
}

void MainApplication::clearFolders(QStringList paths)
{
    auto directoryPaths = paths;
    auto allFiles = fileOperationsService->generateFileObjects(paths);

    auto delegate = DelegateBuilder::buildFileActionEntity<EntityModel>(paths,allFiles,RRT::Delete,QStringList());

    entityQueueManagerService->addEntity(delegate);
}

void MainApplication::clearFoldersAccordingToRules(QStringList paths)
{
    const QList<Rule>rules = ruleManagerService->ruleslist();
    if(rules.isEmpty())
        clearFolders(paths);
    for(const Rule &r : rules)
    {
        FileObjectList allFiles = fileOperationsService->generateFileObjects(paths,r.appliesToPath(),r.typeFilter());
        for(SubRule sR : r.subRules())
            allFiles = fileOperationsService->processFileObjects(allFiles,sR);

        emit sendEntity(
                    DelegateBuilder::buildFileActionEntity<EntityModel>(
                        paths,
                        allFiles,
                        r.actionRuleEntity(),
                        r.destinationPaths()));
    }
}

void MainApplication::configureServices()
{
    // Assign objects to threads
    threadManagerService->createThread(entityQueueManagerService);
    threadManagerService->createThread(fileOperationsService);

    /*
    entityManager->moveToThread(queueThread);
    fWorker->moveToThread(fileWorkerThread);
    */

    // Register custom types that is not recognised by Qt per default
    qRegisterMetaType<DirectoryItem>("DirectoryItem");
    qRegisterMetaType<QList<DirectoryItem>>("QList<DirectoryItem>");
    qRegisterMetaType<EntityModel>("EntityModel");
    qRegisterMetaType<DirectoryEntity>("DirectoryCountEntity");

    /*
     *  Connnections
     */

    // Entity queue related..
    connect(this,&MainApplication::sendEntity,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(entityQueueManagerService,&EntityQueueManager::wakeUpProcess,fileOperationsService,&AbstractFileWorker::handleProcessRequest);
    connect(fileOperationsService,&fW::requestNextEntity,entityQueueManagerService,&AbstractQueueManager::sendNextEntity);
    connect(entityQueueManagerService,&AbstractQueueManager::sendEntity,fileOperationsService,&AbstractFileWorker::processEntity);

    // Detailed directory information..
    connect(settingsManagerService,&AbstractSettingsManager::processPath,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&FileSystemWatcher::folderChanged,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&FileSystemWatcher::sendSystemTrayMessage,this,&MainApplication::sendSystemTrayMessage);
    connect(settingsManagerService,&AbstractSettingsManager::removeItem,fileManagerService,&AbstractFileInformationManager::removeItem);

    connect(fileOperationsService,&fW::processFinished,fileManagerService,&AbstractFileInformationManager::insertItems);
    connect(fileOperationsService,&FileOperationWorker::sendFolderSizeEntity,this,&MainApplication::sendFolderSize);
    connect(fileOperationsService,&FileOperationWorker::sendStatusLineMessage,this,&MainApplication::sendStatusMessage);

    // Notify observers related..
    connect(settingsManagerService,&AbstractSettingsManager::stateChanged,this,&MainApplication::stateChanged);
    connect(ruleManagerService,&rulesManager::stateChanged,this,&MainApplication::stateChanged);
    connect(fileManagerService,&FileInformationManager::stateChanged,this,&MainApplication::stateChanged);

    connect(fileOperationsService,&fW::jobDone,this,&AbstractCoreApplication::stateChanged);
}

void MainApplication::startServices()
{
    // Start threads
    threadManagerService->startAllThreads();

    emit settingsManagerService->processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << watchFolders()));
}

void MainApplication::addWatchFolders(QStringList paths)
{
    settingsManagerService->insertPath(paths);
}

void MainApplication::clearWatchFolders()
{
    settingsManagerService->clearPaths();

}

QString MainApplication::watchFolder(int index) const
{
    return settingsManagerService->paths().value(index);
}

QStringList MainApplication::watchFolders()
{
    return settingsManagerService->paths();
}

int MainApplication::watchFolderCount()
{
    return settingsManagerService->folderCount();
}

void MainApplication::clearRules() const
{
    for (int i = 0; i < ruleManagerService->ruleCount(); ++i)
        ruleManagerService->removeRuleAt(i);
}

const ISettingsDelegate *MainApplication::settingsState()
{
    return settingsManagerService->settingsState();
}

void MainApplication::setSettings(const ISettingsDelegate *s)
{
    settingsManagerService->setSettings(s);
}

void MainApplication::calculateFolderSize(QString path)
{
    QFileInfo fInfo(path);

    auto directoryPath = fInfo.absoluteFilePath();
    auto directoryName = fInfo.fileName();


    entityQueueManagerService->addEntity(DelegateBuilder::buildDirectoryCountEntity<EntityModel>(0,directoryName,directoryPath));
}

void MainApplication::removeWatchFolderAt(int index)
{
    settingsManagerService->removePathAt(index);
}

void MainApplication::removeWatchFolder(QString path)
{
    settingsManagerService->removePath(path);
}
