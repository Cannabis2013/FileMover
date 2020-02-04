#include "mainapplication.h"

MainApplication::MainApplication(const QString &appName,
                                 const QString &orgName)
{
    // Initialize member variables and pointers
    rManager = new rulesManager(appName,orgName);
    sManager = new settingsManager(appName,orgName);
    entityManager = new EntityQueueManager();
    fManager = new FileInformationManager(appName,orgName);
    fWorker = new FileOperationWorker();
    fWatcher = new FileSystemWatcher(sManager->paths());
    tManager = new ThreadsManager();
    fileWorkerThread = new QThread();
    queueThread = new QThread();

    QString ressourceFolderPath = "Ressources";


    // Assign objects to threads
    tManager->createThread(entityManager);
    tManager->createThread(fWorker);

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
    connect(this,&MainApplication::sendEntity,entityManager,&EntityQueueManager::addEntity);
    connect(entityManager,&EntityQueueManager::wakeUpProcess,fWorker,&fW::handleProcessRequest);
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

    connect(fWorker,&fW::jobDone,this,&AbstractCoreApplication::stateChanged);

    // Start threads
    tManager->startAllThreads();

    emit sManager->processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << watchFolders()));
}

MainApplication::~MainApplication()
{
    delete sManager;
    delete fManager;
    delete rManager;
    delete tManager;
}

void MainApplication::clearFolders(QStringList paths)
{
    auto directoryPaths = paths;
    auto allFiles = fW::generateFileObjects(paths);

    auto delegate = DelegateBuilder::buildFileActionEntity<EntityModel>(paths,allFiles,rD::Delete,QStringList());

    entityManager->addEntity(delegate);
}

void MainApplication::clearFoldersAccordingToRules(QStringList paths)
{
    const QList<Rule>rules = rManager->ruleslist();
    if(rules.isEmpty())
        clearFolders(paths);
    for(const Rule &r : rules)
    {
        FileObjectList allFiles = fW::generateFileObjects(paths,r.appliesToPath,r.typeFilter);
        for(SubRule sR : r.subRules)
            allFiles = fWorker->processFileObjects(allFiles,sR);

        emit sendEntity(
                    DelegateBuilder::buildFileActionEntity<EntityModel>(
                        paths,
                        allFiles,
                        r.actionRuleEntity,
                        r.destinationPaths));
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

int MainApplication::watchFolderCount()
{
    return sManager->folderCount();
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

    auto directoryPath = fInfo.absoluteFilePath();
    auto directoryName = fInfo.fileName();


    entityManager->addEntity(DelegateBuilder::buildDirectoryCountEntity<EntityModel>(0,directoryName,directoryPath));
}

void MainApplication::removeWatchFolderAt(int index)
{
    sManager->removePathAt(index);
}

void MainApplication::removeWatchFolder(QString path)
{
    sManager->removePath(path);
}
