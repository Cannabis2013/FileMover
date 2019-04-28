#include "settingsmanager.h"

settingsManager::settingsManager(QString appName, QString orgName):
    AbstractPersistence (appName,orgName)
{
    QList<MyIcon> trayIconList = iconsFromPath(rPaths.ressourceFolder);

    QDir dir;
    if(!dir.exists(rPaths.ressourceFolder))
        dir.mkdir(rPaths.ressourceFolder);
    if(!dir.exists(rPaths.fileIconPath))
        dir.mkdir(rPaths.fileIconPath);

    if(trayIconList.count() == 0)
        settings.currentTrayIcon = QIcon(":/My Images/Ressources/Hdd-icon.png");
    else
    {
        insertIcons(trayIconList);
        settings.currentTrayIcon = trayIconList.last();
    }

    settings.fileIconList = iconsFromPath(rPaths.fileIconPath);
    if(!settings.fileIconList.isEmpty())
        settings.fileIconStandard = settings.fileIconList.last();
}

void settingsManager::insertPath(QString path)
{
    settings.mainFolderPaths << path;
    emit stateChanged();
}

void settingsManager::insertPaths(QStringList paths)
{
    settings.mainFolderPaths << paths;
    emit stateChanged();
}

void settingsManager::readSettings()
{
    QSettings settings(OrganisationTitle(),ApplicationTitle());
}

void settingsManager::writeSettings()
{

}
