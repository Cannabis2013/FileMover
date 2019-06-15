#include "mainapplication.h"

MainApplication::MainApplication(QString appName, QString orgName)
{
    rManager = new rulesManager(appName,orgName);
    sManager = new settingsManager(appName,orgName);
    pManager = new ProcessManager();
    fManager = new FileInformationManager(appName,orgName);
    fWorker = new FileWorker();
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
    auto freshList = [paths](QString rPath = QString(),bool recursive = false)->QFileInfoList
    {
        QStringList rPaths;
        if(rPath != QString() && rPath != "Alle")
            rPaths = QStringList(rPath);
        else
            rPaths = paths;
        QFileInfoList allFiles;
        if(!recursive)
        /*Add items to a "QFileInfoList" in a non-recursive manner,
         * which means that directories, excluding their content, will be added
         */
        {
            for(QString path : rPaths)
            {
                QDir dirContent(path);
                allFiles += dirContent.entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System);
            }
        }
        // Add files to a QFileInfoList in a recursive manner, but excludes directories
        else
        {
            for(QString path : paths)
            {
                QDirIterator it(path,
                                QDir::AllEntries | QDir::NoDotAndDotDot | QDir::System,
                                QDirIterator::Subdirectories);
                while(it.hasNext())
                {
                    QFileInfo fileItem = it.next();
                    if(!fileItem.isDir())
                        allFiles.append(fileItem);
                }
            }
        }
        return allFiles;
    };
    auto outputFiles = [](QFileInfoList list)->void
    {
        for(QFileInfo file : list)
        {
            QString fileSizeUnit;
            double sz = fW::convertSizeToAppropriateUnits(file.size(),fileSizeUnit);
            QString fileSize = QString::number(sz) + " " + fileSizeUnit;
            cout << "Filename: " << file.fileName().toStdString() << " Filepath: " << file.filePath().toStdString() <<
                 " Size: " << fileSize.toStdString() << endl;
        }
    };
    Q_UNUSED(outputFiles);
    QList<Rule>rules = rManager->ruleslist();
    for(Rule r : rules)
    {
        QFileInfoList allFiles = freshList(r.appliesToPath,r.deepScanMode);
        for(SubRule sR : r.subRules)
            allFiles = fWorker->processList(allFiles,sR);

        // Implementer fileworker operation..

        ProcessItems it;
        it.destinations = r.destinationPath;
        it.list = allFiles;
        it.ruleMode = r.actionRule;
        pManager->addToQueue(it);
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
