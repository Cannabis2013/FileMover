#include "settingsmanager.h"

settingsManager::settingsManager(const QString &appName, const QString &orgName):
    AbstractPersistence (appName,orgName)
{
    QList<MyIcon> trayIconList = scanForIcons(ressourceFolder);

    _settings = new SettingsDelegate;

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

    fileIconList = scanForIcons(fileIconPath);
    if(!fileIconList.isEmpty())
        fileIconStandard = fileIconList.last();

    readSettings();
    deserialize();
}

settingsManager::~settingsManager()
{
    writeSettings();
    serialize();
}

void settingsManager::insertPath(QString path)
{
    mainFolderPaths << path;

    emit processPath(new fileInformationEntity(path));
    emit stateChanged();
}

void settingsManager::insertPath(QStringList paths)
{
    mainFolderPaths << paths;
    emit processPath(new fileInformationEntity(paths));
    emit stateChanged();
}

void settingsManager::removePath(QString path)
{
    mainFolderPaths.removeOne(path);
    emit removeItem(path);
    emit stateChanged();
}

void settingsManager::removePathAt(int index)
{
    mainFolderPaths.removeAt(index);
    emit stateChanged();
}

void settingsManager::requestProcess()
{

}

QList<QTreeWidgetItem *> settingsManager::pathItems()
{
    QList<QTreeWidgetItem*> items;
    for (QString path : mainFolderPaths) {
        QTreeWidgetItem *treeItem = new QTreeWidgetItem();
        treeItem->setIcon(0,QIcon(":/My Images/Ressources/Folder.png"));
        treeItem->setText(0,path);
        items << treeItem;
    }

    return items;
}

QList<MyIcon> settingsManager::scanForIcons(QString path)
{
    QList<MyIcon>icons;
    QDirIterator iT(path);
    while(iT.hasNext())
    {
        QFileInfo file = iT.next();
#ifdef Q_OS_WIN32
        if(file.suffix() == "ico" || file.suffix() == "png")
        {
            QString fP = file.absoluteFilePath(),fN = file.fileName();
            MyIcon ico(fP);
            ico.setName(fN);
            icons << ico;
        }
#elif defined Q_OS_MAC
        if(file.suffix() == "icns")
        {
            QString fP = file.absoluteFilePath(),fN = file.fileName();
            myIcon ico(fP);
            ico.setName(fN);
            icons << ico;
        }
#endif
    }
    return icons;
}

void settingsManager::readSettings()
{
    persistenceSettings->beginGroup("Basic settings");

    _settings->closeOnExit = persistenceSettings->value("Close on exit",true).toBool();
    _settings->ruleCountInterval = persistenceSettings->value("Count timer interval", 2000).toInt();
    _settings->rulesEnabled = persistenceSettings->value("Rules enabled", false).toBool();
    _settings->ruleTimerEnabled = persistenceSettings->value("Timer enabled",false).toBool();
    _settings->mainGuiGeometry = persistenceSettings->value("Main gui geometry",QRect()).toRect();

    persistenceSettings->endGroup();

    int count = persistenceSettings->beginReadArray("Watchfolders");
    QStringList folders;
    for (int i = 0;i < count;i++)
    {
        persistenceSettings->setArrayIndex(i);
        folders << persistenceSettings->value(QString("Folder (%1)").arg(i)).toString();
    }

    insertPath(folders);
    persistenceSettings->endArray();
}

void settingsManager::writeSettings()
{
    persistenceSettings->beginGroup("Basic settings");
    persistenceSettings->clear();
    persistenceSettings->setValue("Close on exit", _settings->closeOnExit);
    persistenceSettings->setValue("Count timer interval", _settings->ruleCountInterval);
    persistenceSettings->setValue("Rules enabled", _settings->rulesEnabled);
    persistenceSettings->setValue("Timer enabled",_settings->ruleTimerEnabled);
    persistenceSettings->setValue("Main gui geometry",_settings->mainGuiGeometry);
    persistenceSettings->endGroup();
    persistenceSettings->beginWriteArray("Watchfolders", mainFolderPaths.count());

    for(int i = 0;i < mainFolderPaths.count();i++)
    {
        persistenceSettings->setArrayIndex(i);
        QString path = mainFolderPaths.at(i);
        QString keyVal = QString("Folder (%1)").arg(i);
        persistenceSettings->setValue(keyVal,path);
    }
    persistenceSettings->endArray();
}

void settingsManager::setCloseOnExit(bool enable)
{
    _settings->closeOnExit = enable;
}

void settingsManager::setRulesEnabled(bool enable)
{
    _settings->rulesEnabled = enable;
}

void settingsManager::setTimerEnabled(bool enable)
{
    _settings->ruleTimerEnabled = enable;
}

void settingsManager::setTimerInterval(int msec)
{
    _settings->ruleCountInterval = msec;
}

SettingsDelegate settingsManager::settingsState()
{
    return *_settings;
}

void settingsManager::setSettings(SettingsDelegate s)
{
    *_settings = s;
}
