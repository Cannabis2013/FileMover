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
}

settingsManager::~settingsManager()
{
    writeSettings();
}

void settingsManager::insertPath(QString path)
{
    watchFolders << path;
    FileInformationEntity *fEntity;
    try {
        fEntity = eMD::makeEntity<FileInformationEntity>(EntityModel::fileInformationEntity);
    } catch (const char *msg) {
        cout << msg << endl;
        exit(1);
    }
    fEntity->filePaths << path;
    emit processPath(fEntity);
    emit stateChanged();
}

void settingsManager::insertPath(const QStringList& paths)
{
    watchFolders << paths;
    FileInformationEntity *fEntity;
    try {
        fEntity = eMD::makeEntity<FileInformationEntity>(EntityModel::fileInformationEntity);
    } catch (const char *msg) {
        cout << msg << endl;
        exit(1);
    }
    fEntity->filePaths = paths;
    emit processPath(fEntity);
    emit stateChanged();
}

void settingsManager::removePath(QString path)
{
    watchFolders.removeOne(path);
    emit removeItem(path);
    emit stateChanged();
}

void settingsManager::removePathAt(int index)
{
    // TODO: Implement some 'out of range' check
    // NOTE: Should throw something
    watchFolders.removeAt(index);
    emit stateChanged();
}

void settingsManager::clearPaths()
{
    for (int i = 0; i < watchFolders.count(); ++i)
        removePathAt(i);
}

void settingsManager::requestProcess()
{

}

QList<QTreeWidgetItem *> settingsManager::pathItems()
{
    QList<QTreeWidgetItem*> items;
    for (const QString &path : watchFolders) {
        auto treeItem = new QTreeWidgetItem();
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
    QSettings *pSettings = persistenceSettings();
    pSettings->beginGroup("Basic settings");

    _settings->closeOnExit = pSettings->value("Close on exit",true).toBool();
    _settings->ruleCountInterval = pSettings->value("Count timer interval", 2000).toInt();
    _settings->rulesEnabled = pSettings->value("Rules enabled", false).toBool();
    _settings->ruleTimerEnabled = pSettings->value("Timer enabled",false).toBool();
    _settings->mainGuiGeometry = pSettings->value("Main gui geometry",QRect()).toRect();

    pSettings->endGroup();

    int count = pSettings->beginReadArray("Watchfolders");
    QStringList folders;
    for (int i = 0;i < count;i++)
    {
        pSettings->setArrayIndex(i);
        folders << pSettings->value(QString("Folder (%1)").arg(i)).toString();
    }

    insertPath(folders);
    pSettings->endArray();
}

void settingsManager::writeSettings()
{
    QSettings *pSettings = persistenceSettings();

    pSettings->beginGroup("Basic settings");
    pSettings->clear();
    pSettings->setValue("Close on exit", _settings->closeOnExit);
    pSettings->setValue("Count timer interval", _settings->ruleCountInterval);
    pSettings->setValue("Rules enabled", _settings->rulesEnabled);
    pSettings->setValue("Timer enabled",_settings->ruleTimerEnabled);
    pSettings->setValue("Main gui geometry",_settings->mainGuiGeometry);
    pSettings->endGroup();
    pSettings->beginWriteArray("Watchfolders", watchFolders.count());

    for(int i = 0;i < watchFolders.count();i++)
    {
        pSettings->setArrayIndex(i);
        QString path = watchFolders.at(i);
        QString keyVal = QString("Folder (%1)").arg(i);
        pSettings->setValue(keyVal,path);
    }
    pSettings->endArray();
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
