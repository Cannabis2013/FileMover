#include "applicationdomain.h"

ApplicationDomain::ApplicationDomain()
{
}

ApplicationDomain::~ApplicationDomain()
{
    delete settingsService;
    delete informationService;
    delete rulesService;
}

void ApplicationDomain::clearFolders(QStringList paths)
{
    auto models = fileListService->fileModelDelegates(paths,FilesContext::All);
    auto delegates = DelegateBuilder::buildFileActionEntity<EntityModel>(paths,models,FilesContext::Delete,QStringList());
    queueService->addEntity(delegates);
}

void ApplicationDomain::clearFoldersAccordingToRules(QStringList paths)
{
    for (auto rule : rulesService->rules()) {

    }
}

void ApplicationDomain::configureServices()
{
    fileWatcherService->addPaths(settingsService->paths());

    // Assign objects to threads
    threadingService->createThread(queueService);
    threadingService->createThread(fileOperationsService);

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
    connect(this,&ApplicationDomain::sendEntity,queueService,&AbstractQueueManager::addEntity);
    connect(queueService,&AbstractQueueManager::wakeUpProcess,fileOperationsService,&AbstractFileWorker::handleProcessRequest);
    connect(fileOperationsService,&AbstractFileWorker::requestNextEntity,queueService,&AbstractQueueManager::sendNextEntity);
    connect(queueService,&AbstractQueueManager::sendEntity,fileOperationsService,&AbstractFileWorker::processEntity);

    // Detailed directory information..
    connect(settingsService,&AbstractSettingsManager::processPath,queueService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&AbstractFileSystemWatcher::folderChanged,queueService,&AbstractQueueManager::addEntity);
    connect(fileWatcherService,&AbstractFileSystemWatcher::sendSystemTrayMessage,this,&ApplicationDomain::sendSystemTrayMessage);
    connect(settingsService,&AbstractSettingsManager::removeItem,informationService,&AbstractFileInformationManager::removeItem);

    connect(fileOperationsService,&AbstractFileWorker::processFinished,informationService,&AbstractFileInformationManager::insertItems);
    connect(fileOperationsService,&AbstractFileWorker::sendFolderSizeEntity,this,&ApplicationDomain::sendFolderSize);
    connect(fileOperationsService,&AbstractFileWorker::sendStatusLineMessage,this,&ApplicationDomain::sendStatusMessage);

    // Notify observers related..
    connect(settingsService,&AbstractSettingsManager::stateChanged,this,&ApplicationDomain::stateChanged);
    connect(rulesService,&AbstractRulesManager::stateChanged,this,&ApplicationDomain::stateChanged);
    connect(informationService,&AbstractFileInformationManager::stateChanged,this,&ApplicationDomain::stateChanged);

    connect(fileOperationsService,&AbstractFileWorker::jobDone,this,&AbstractApplicationService::stateChanged);
}

void ApplicationDomain::startServices()
{
    // Start threads
    threadingService->startAllThreads();

    emit settingsService->processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << watchFolders()));
}

void ApplicationDomain::addWatchFolders(QStringList paths)
{
    settingsService->insertPath(paths);
}

void ApplicationDomain::clearWatchFolders()
{
    settingsService->clearPaths();

}

QString ApplicationDomain::watchFolder(int index) const
{
    return settingsService->paths().value(index);
}

QStringList ApplicationDomain::watchFolders()
{
    return settingsService->paths();
}

int ApplicationDomain::watchFolderCount()
{
    return settingsService->folderCount();
}

void ApplicationDomain::clearRules() const
{
    for (int i = 0; i < rulesService->rulesCount(); ++i)
        rulesService->removeRuleAt(i);
}

const ISettingsDelegate *ApplicationDomain::settingsState()
{
    return settingsService->settingsState();
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

    settingsService->setSettings(settingsDelegate);
}

void ApplicationDomain::calculateFolderSize(QString path)
{
    QFileInfo fInfo(path);

    auto directoryPath = fInfo.absoluteFilePath();
    auto directoryName = fInfo.fileName();


    queueService->addEntity(DelegateBuilder::buildDirectoryCountEntity<EntityModel>(0,directoryName,directoryPath));
}

void ApplicationDomain::removeWatchFolderAt(int index)
{
    settingsService->removePathAt(index);
}

void ApplicationDomain::removeWatchFolder(QString path)
{
    settingsService->removePath(path);
}

void ApplicationDomain::setFilteringContext(DefaultFilteringContext *service)
{

}
