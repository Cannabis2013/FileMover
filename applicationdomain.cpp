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
    auto models = fileListService->buildFileModels(FilesContext::All,paths);
    auto delegates = DelegateBuilder::buildFileActionEntity<EntityModel>(paths,models,FilesContext::Delete,QStringList());
    queueService->addEntity(delegates);
}

void ApplicationDomain::clearFoldersAccordingToRules(QStringList paths)
{
    auto folders = watchFolders();
    fileListService->appendFileLists(folders);
    auto list = filteringService->process(rulesService->rules());
    for (auto delegate : list) {
        emit sendEntity(delegate);
    }
}

AbstractApplicationService *ApplicationDomain::configureServices()
{
    fileWatcherService->addPaths(settingsService->paths());

    // Assign objects to threads
    threadingService->createThread(queueService);
    threadingService->createThread(fileOperationsService);

    filteringService->setListService(fileListService);

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

    return this;
}

AbstractApplicationService* ApplicationDomain::startServices()
{
    // Start threads
    threadingService->startAllThreads();

    emit settingsService->processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << watchFolders()));

    return this;
}

AbstractApplicationService *ApplicationDomain::setRuleManagerService(AbstractRulesManager *service)
{
    rulesService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setSettingsManagerService(AbstractSettingsManager *service)
{
    settingsService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setEntityQueueManagerService(AbstractQueueManager *service)
{
    queueService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setFileInformationManagerService(AbstractFileInformationManager *service)
{
    informationService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setThreadManagerService(IThreadManagerInterface *service)
{
    threadingService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setFileOperationsService(AbstractFileWorker *service)
{
    fileOperationsService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setFileModelBuilderService(IFileListService<IModelBuilder<IFileModel<>, QString> > *service)
{
    fileListService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setFileWatcherService(AbstractFileSystemWatcher *service)
{
    fileWatcherService = service;
    return this;
}

AbstractApplicationService *ApplicationDomain::setRuleDefinitionsService(IRuleDefinitions *service)
{
    _ruleDefinitionsService = service;
    return this;
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

AbstractApplicationService *ApplicationDomain::setFilteringContext(DefaultFilteringContext *filterService, DefaulFileList *listService)
{
    filteringService = filterService;
    filteringService->setListService(listService);
    fileListService = listService;

    return this;
}
