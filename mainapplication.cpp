#include "mainapplication.h"



MainApplication::MainApplication(QString appName, QString orgName):
    AbstractPersistence(appName, orgName)
{
    rManager = new rulesManager();
    sManager = new settingsManager(appName,orgName);
    closeOnBut = true;

    bool rulesEnabled= true;

    QString ressourceFolderPath = "Ressources";

    /*
     * Initialize SettingsManager
     */

    sManager->setCloseOnExit(closeOnBut);
    sManager->setRulesEnabled(rulesEnabled);
}

MainApplication::~MainApplication()
{

}

void MainApplication::readSettings()
{

}

void MainApplication::writeSettings()
{

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
    QList<rule>rules = rManager.ruleslist();
    for(rule r : rules)
    {
        QFileInfoList allFiles = freshList(r.appliesToPath,r.deepScanMode);
        for(subRule sR : r.subRules)
            allFiles = sR.processList(allFiles);

        // Implementer fileworker operation..

        processItems it;
        it.destinations = r.destinationPath;
        it.list = allFiles;
        it.ruleMode = r.actionRule;
        pManager->addToQueue(it);
    }

}

void MainApplication::addWatchFolder(QString path)
{
    sManager->insertPath(path);
}

QString MainApplication::watchFolder(int index)
{

}

QStringList MainApplication::watchFolders()
{
    return sManager->paths();
}

