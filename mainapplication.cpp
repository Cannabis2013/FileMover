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
    fileWorkerThread = new QThread();
    queueThread = new QThread();

    QString ressourceFolderPath = "Ressources";


    // Move classes to their respective threads
    entityManager->moveToThread(queueThread);
    fWorker->moveToThread(fileWorkerThread);

    // Register custom types that is not recognised by Qt per default
    qRegisterMetaType<DirectoryItem>("DirectoryItem");
    qRegisterMetaType<QList<DirectoryItem>>("QList<DirectoryItem>");
    qRegisterMetaType<EntityModel>("EntityModel");
    qRegisterMetaType<DirectoryCountEntity>("DirectoryCountEntity");

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

    connect(fWorker,&fW::jobDone,this,&ICoreApplication::stateChanged);

    // Start threads
    fileWorkerThread->start();
    queueThread->start();

    // Incarcerate entity model
    FileInformationEntity *fEntity;
    try {
        fEntity = makeEntity<FileInformationEntity>(EntityModel::fileInformationEntity);
    } catch (const char *msg) {
        cout << msg << endl;
        exit(1);
    }

    fEntity->filePaths = watchFolders();

    emit sManager->processPath(fEntity);
}

MainApplication::~MainApplication()
{
    delete sManager;
    delete fManager;
    delete rManager;
}

void MainApplication::clearFolders(QStringList paths)
{
    FileActionEntity *file_entity;
    try {
        file_entity = makeEntity<FileActionEntity>(EntityModel::fileOperationEntity);
    } catch (const char *msg) {
        cout << msg << endl;
        exit(1);
    }

    file_entity->directoryPaths = paths;
    file_entity->allFiles = fW::generateFileObjects(paths);
    entityManager->addEntity(file_entity);
}

void MainApplication::clearFoldersAccordingToRules(QStringList paths)
{
    const QList<Rule>rules = rManager->ruleslist();
    if(rules.isEmpty())
        clearFolders(paths);
    for(const Rule &r : rules)
    {
        QList<const IFileModelDelegate<IFileModel<FileModel>>*> allFiles = fW::generateFileObjects(paths,r.appliesToPath,r.typeFilter);
        for(SubRule sR : r.subRules)
            allFiles = fWorker->processFileObjects(allFiles,sR);

        FileActionEntity *file_entity;
        try {
            file_entity = makeEntity<FileActionEntity>(EntityModel::fileOperationEntity);
        } catch (const char *msg) {
            cout << msg << endl;
            exit(1);
        }
        file_entity->directoryPaths = paths;
        file_entity->fileDestinations = r.destinationPaths;
        file_entity->allFiles = allFiles;
        file_entity->fileActionRule = r.actionRuleEntity;
        emit sendEntity(file_entity);
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

Settings_Structure MainApplication::settingsState()
{
    return sManager->settingsState();
}

void MainApplication::setSettings(Settings_Structure s)
{
    sManager->setSettings(s);
}

void MainApplication::calculateFolderSize(QString path)
{
    QFileInfo fInfo(path);
    DirectoryCountEntity *dirEntity;
    try {
        dirEntity = makeEntity<DirectoryCountEntity>(EntityModel::directoryCountEntity);
    } catch (const char *msg) {
        cout << msg << endl;
        exit(1);
    }
    dirEntity->directoryPath = fInfo.absoluteFilePath();
    dirEntity->directoryName = fInfo.fileName();

    entityManager->addEntity(dirEntity);
}

void MainApplication::removeWatchFolderAt(int index)
{
    sManager->removePathAt(index);
}

void MainApplication::removeWatchFolder(QString path)
{
    sManager->removePath(path);
}
