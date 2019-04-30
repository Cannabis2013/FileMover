#include "settingsmanager.h"

settingsManager::settingsManager(QString appName, QString orgName):
    AbstractPersistence (appName,orgName)
{
    QList<MyIcon> trayIconList = iconsFromPath(ressourceFolder);

    QDir dir;
    if(!dir.exists(ressourceFolder))
        dir.mkdir(ressourceFolder);
    if(!dir.exists(fileIconPath))
        dir.mkdir(fileIconPath);

    if(trayIconList.count() == 0)
        currentTrayIcon = QIcon(":/My Images/Ressources/Hdd-icon.png");
    else
    {
        insertIcons(trayIconList);
        currentTrayIcon = trayIconList.last();
    }

    fileIconList = iconsFromPath(fileIconPath);
    if(!fileIconList.isEmpty())
        fileIconStandard = fileIconList.last();
}

settingsManager::~settingsManager()
{
    writeSettings();
}

void settingsManager::insertPath(QString path)
{
    mainFolderPaths << path;
    emit stateChanged();
}

void settingsManager::insertPaths(QStringList paths)
{
    mainFolderPaths << paths;
    emit stateChanged();
}

void settingsManager::removePath(QString path)
{
    mainFolderPaths.removeOne(path);
    emit stateChanged();
}

void settingsManager::removePathAt(int index)
{
    mainFolderPaths.removeAt(index);
    emit stateChanged();
}

QList<QTreeWidgetItem *> settingsManager::pathItems()
{
    QList<QTreeWidgetItem*> items;
    for (QString path : mainFolderPaths) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();
        treeItem->setText(0,path);
        items << treeItem;
    }
    return items;
}

void settingsManager::readSettings()
{
    QSettings settings(OrganisationTitle(),ApplicationTitle());
}

void settingsManager::writeSettings()
{
    QSettings s(OrganisationTitle(),ApplicationTitle());

    s.beginGroup("Settings");

    s.setValue("Close on exit", closeOnExit);
    s.setValue("Count timer interval", timerMsec);
    s.setValue("Rules enabled", rulesEnabled);

    s.endGroup();

    int index = 0;
    s.beginGroup("WatchFolders");
    for(QString path : paths())
    {
        QString keyVal = QString("Folder (%1)").arg(index++);
        s.setValue(keyVal,path);
    }
    s.endGroup();
}
