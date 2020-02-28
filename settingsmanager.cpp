#include "settingsmanager.h"

settingsManager::settingsManager(const QString &appName, const QString &orgName):
    AbstractPersistence (appName,orgName)
{
    QList<const AbstractIcon*> trayIconList = scanForIcons(ressourceFolder);

    // Allocate memmory and initialize later
    _settings = SettingsDelegateBuilder::buildDefaultDelegate();

    QDir dir;
    if(!dir.exists(ressourceFolder))
        dir.mkdir(ressourceFolder);
    if(!dir.exists(fileIconPath))
        dir.mkdir(fileIconPath);

    if(trayIconList.count() == 0)
        currentTrayIcon = new Icon(":/My Images/Ressources/Hdd-icon.png");
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

    emit processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << path));
    emit stateChanged();
}

void settingsManager::insertPath(const QStringList& paths)
{
    watchFolders << paths;

    emit processPath(
                DelegateBuilder::buildFileInformationEntity<EntityModel>(
                    QStringList() << paths));
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

QList<QTreeWidgetItem *> settingsManager::pathItems() const
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

QList<const AbstractIcon*> settingsManager::scanForIcons(QString path)
{
    QList<const AbstractIcon *>icons;
    QDirIterator iT(path);
    while(iT.hasNext())
    {
        QFileInfo file = iT.next();
#ifdef Q_OS_WIN32
        if(file.suffix() == "ico" || file.suffix() == "png")
        {
            QString fP = file.absoluteFilePath(),fN = file.fileName();
            auto ico = new Icon(fP);
            ico->setName(fN);
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

    auto closeOnExit = pSettings->value("Close on exit",true).toBool();
    auto ruleCountInterval = pSettings->value("Count timer interval", 2000).toInt();
    auto rulesEnabled = pSettings->value("Rules enabled", false).toBool();
    auto ruleTimerEnabled = pSettings->value("Timer enabled",false).toBool();
    auto mainGuiGeometry = pSettings->value("Main gui geometry",QRect()).toRect();

    pSettings->endGroup();

    _settings = SettingsDelegateBuilder::buildSettingsDelegate(closeOnExit,
                                                               ruleTimerEnabled,
                                                               rulesEnabled,
                                                               ruleCountInterval,
                                                               mainGuiGeometry);

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
    pSettings->setValue("Close on exit", _settings->closeOnExit());
    pSettings->setValue("Count timer interval", _settings->ruleCountInterval());
    pSettings->setValue("Rules enabled", _settings->rulesEnabled());
    pSettings->setValue("Timer enabled",_settings->ruleTimerEnabled());
    pSettings->setValue("Main gui geometry",_settings->mainGuiGeometry());
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
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(enable,
                                                            _settings->ruleTimerEnabled(),
                                                            _settings->rulesEnabled(),
                                                            _settings->ruleCountInterval(),
                                                            _settings->mainGuiGeometry());
}

void settingsManager::setRulesEnabled(bool enable)
{
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(_settings->closeOnExit(),
                                                            _settings->ruleTimerEnabled(),
                                                            enable,
                                                            _settings->ruleCountInterval(),
                                                            _settings->mainGuiGeometry());
}

void settingsManager::setTimerEnabled(bool enable)
{
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(_settings->closeOnExit(),
                                                            enable,
                                                            _settings->rulesEnabled(),
                                                            _settings->ruleCountInterval(),
                                                            _settings->mainGuiGeometry());
}

void settingsManager::setTimerInterval(int msec)
{
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(_settings->closeOnExit(),
                                                            _settings->ruleTimerEnabled(),
                                                            _settings->rulesEnabled(),
                                                            msec,
                                                            _settings->mainGuiGeometry());
}

const ISettingsDelegate *settingsManager::settingsState() const
{
    return _settings;
}

void settingsManager::setSettings(const ISettingsDelegate *s)
{
    _settings = s;
}
