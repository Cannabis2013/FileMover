#include "applicationdomain.h"

ApplicationDomain::ApplicationDomain()
{
}

ApplicationDomain::~ApplicationDomain()
{
    delete settingsManagerService;
    delete fileManagerService;
    delete ruleManagerService;
}

void ApplicationDomain::clearFolders(QStringList paths)
{
    auto allFiles = fileOperationsService->generateFileObjects(paths);
    auto delegate = DelegateBuilder::buildFileActionEntity<EntityModel>(paths,allFiles,RRT::Delete,QStringList());
    entityQueueManagerService->addEntity(delegate);
}

void ApplicationDomain::clearFoldersAccordingToRules(QStringList paths)
{
    auto rules = ruleManagerService->ruleslist();
    if(rules.isEmpty())
        clearFolders(paths);
    for(const Rule &r : rules)
    {
        auto allFiles = fileOperationsService->generateFileObjects(paths,r.appliesToPath(),r.typeFilter());
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

void ApplicationDomain::configureServices()
{
    fileWatcherService->addPaths(settingsManagerService->paths());

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
    connect(this,&ApplicationDomain::sendEntity,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(entityQueueManagerService,&EntityQueueManager::wakeUpProcess,fileOperationsService,&AbstractFileWorker::handleProcessRequest);
    connect(fileOperationsService,&fW::requestNextEntity,entityQueueManagerService,&AbstractQueueManager::sendNextEntity);
    connect(entityQueueManagerService,&AbstractQueueManager::sendEntity,fileOperationsService,&AbstractFileWorker::processEntity);

    // Detailed directory information..
    connect(settingsManagerService,&AbstractSettingsManager::processPath,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&FileSystemWatcher::folderChanged,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&FileSystemWatcher::sendSystemTrayMessage,this,&ApplicationDomain::sendSystemTrayMessage);
    connect(settingsManagerService,&AbstractSettingsManager::removeItem,fileManagerService,&AbstractFileInformationManager::removeItem);

    connect(fileOperationsService,&fW::processFinished,fileManagerService,&AbstractFileInformationManager::insertItems);
    connect(fileOperationsService,&FileWorker::sendFolderSizeEntity,this,&ApplicationDomain::sendFolderSize);
    connect(fileOperationsService,&FileWorker::sendStatusLineMessage,this,&ApplicationDomain::sendStatusMessage);

    // Notify observers related..
    connect(settingsManagerService,&AbstractSettingsManager::stateChanged,this,&ApplicationDomain::stateChanged);
    connect(ruleManagerService,&rulesManager::stateChanged,this,&ApplicationDomain::stateChanged);
    connect(fileManagerService,&FileInformationManager::stateChanged,this,&ApplicationDomain::stateChanged);

    connect(fileOperationsService,&fW::jobDone,this,&AbstractApplicationService::stateChanged);
}

void ApplicationDomain::startServices()
{
    // Start threads
    threadManagerService->startAllThreads();

    emit settingsManagerService->processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << watchFolders()));
}

void ApplicationDomain::addWatchFolders(QStringList paths)
{
    settingsManagerService->insertPath(paths);
}

void ApplicationDomain::clearWatchFolders()
{
    settingsManagerService->clearPaths();

}

QString ApplicationDomain::watchFolder(int index) const
{
    return settingsManagerService->paths().value(index);
}

QStringList ApplicationDomain::watchFolders()
{
    return settingsManagerService->paths();
}

int ApplicationDomain::watchFolderCount()
{
    return settingsManagerService->folderCount();
}

void ApplicationDomain::clearRules() const
{
    for (int i = 0; i < ruleManagerService->ruleCount(); ++i)
        ruleManagerService->removeRuleAt(i);
}

const ISettingsDelegate *ApplicationDomain::settingsState()
{
    return settingsManagerService->settingsState();
}

void ApplicationDomain::setSettings(const ISettingsDelegate *s)
{
    settingsManagerService->setSettings(s);
}

void ApplicationDomain::calculateFolderSize(QString path)
{
    QFileInfo fInfo(path);

    auto directoryPath = fInfo.absoluteFilePath();
    auto directoryName = fInfo.fileName();


    entityQueueManagerService->addEntity(DelegateBuilder::buildDirectoryCountEntity<EntityModel>(0,directoryName,directoryPath));
}

void ApplicationDomain::removeWatchFolderAt(int index)
{
    settingsManagerService->removePathAt(index);
}

void ApplicationDomain::removeWatchFolder(QString path)
{
    settingsManagerService->removePath(path);
}
