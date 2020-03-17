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
    auto models = fileModelBuilderService->fileModelDelegates(paths,FilesContext::All);
    auto delegates = DelegateBuilder::buildFileActionEntity<EntityModel>(paths,models,FilesContext::Delete,QStringList());
    entityQueueManagerService->addEntity(delegates);
}

void ApplicationDomain::clearFoldersAccordingToRules(QStringList paths)
{

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
    connect(entityQueueManagerService,&AbstractQueueManager::wakeUpProcess,fileOperationsService,&AbstractFileWorker::handleProcessRequest);
    connect(fileOperationsService,&AbstractFileWorker::requestNextEntity,entityQueueManagerService,&AbstractQueueManager::sendNextEntity);
    connect(entityQueueManagerService,&AbstractQueueManager::sendEntity,fileOperationsService,&AbstractFileWorker::processEntity);

    // Detailed directory information..
    connect(settingsManagerService,&AbstractSettingsManager::processPath,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&AbstractFileSystemWatcher::folderChanged,entityQueueManagerService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&AbstractFileSystemWatcher::sendSystemTrayMessage,this,&ApplicationDomain::sendSystemTrayMessage);
    connect(settingsManagerService,&AbstractSettingsManager::removeItem,fileManagerService,&AbstractFileInformationManager::removeItem);

    connect(fileOperationsService,&AbstractFileWorker::processFinished,fileManagerService,&AbstractFileInformationManager::insertItems);
    connect(fileOperationsService,&AbstractFileWorker::sendFolderSizeEntity,this,&ApplicationDomain::sendFolderSize);
    connect(fileOperationsService,&AbstractFileWorker::sendStatusLineMessage,this,&ApplicationDomain::sendStatusMessage);

    // Notify observers related..
    connect(settingsManagerService,&AbstractSettingsManager::stateChanged,this,&ApplicationDomain::stateChanged);
    connect(ruleManagerService,&AbstractRulesManager::stateChanged,this,&ApplicationDomain::stateChanged);
    connect(fileManagerService,&AbstractFileInformationManager::stateChanged,this,&ApplicationDomain::stateChanged);

    connect(fileOperationsService,&AbstractFileWorker::jobDone,this,&AbstractApplicationService::stateChanged);
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
    for (int i = 0; i < ruleManagerService->rulesCount(); ++i)
        ruleManagerService->removeRuleAt(i);
}

const ISettingsDelegate *ApplicationDomain::settingsState()
{
    return settingsManagerService->settingsState();
}

void ApplicationDomain::setSettings(const bool &closeOnExit,
                                    const bool &ruleTimerEnabled,
                                    const bool &rulesEnabled,
                                    const QRect &geometry,
                                    const int &countInterval)

{
    auto settingsDelegate = SettingsDelegateBuilder::buildSettingsDelegate(closeOnExit,
                                                                   ruleTimerEnabled,
                                                                   rulesEnabled,
                                                                   countInterval,
                                                                   geometry);

    settingsManagerService->setSettings(settingsDelegate);
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
