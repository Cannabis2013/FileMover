#include "settingsmanager.h"

settingsManager::settingsManager(const QString &appName, const QString &orgName,
                                 ISettingsBuilder<QRect> *builderService,
                                 IEntityModelBuilder<DefaultDelegate> *delegateBuilderService):
    AbstractPersistence (appName,orgName),
    settingsBuilder(builderService),
    _delegateBuilderService(delegateBuilderService)
{
    QList<const AbstractIcon*> trayIconList = scanForIcons(ressourceFolder);

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
    auto paths = QStringList() << path;
    auto delegate = _delegateBuilderService->buildFileInformationDelegate(paths);

    emit processPath(delegate);
    emit stateChanged();
}

void settingsManager::insertPath(const QStringList& paths)
{
    watchFolders << paths;
    auto delegate = _delegateBuilderService->buildFileInformationDelegate(paths);
    emit processPath(delegate);
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

    _settings = settingsBuilder->buildSettings(closeOnExit,
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
    pSettings->setValue("Close on exit", _settings->isCloseOnExitEnabled());
    pSettings->setValue("Count timer interval", _settings->ruleTimerInterval());
    pSettings->setValue("Rules enabled", _settings->isRulesEnabled());
    pSettings->setValue("Timer enabled",_settings->isRuleTimerEnabled());
    pSettings->setValue("Main gui geometry",_settings->geometry());
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
                                                            _settings->isRuleTimerEnabled(),
                                                            _settings->isRulesEnabled(),
                                                            _settings->ruleTimerInterval(),
                                                            _settings->geometry());
}

void settingsManager::setRulesEnabled(bool enable)
{
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(_settings->isCloseOnExitEnabled(),
                                                            _settings->isRuleTimerEnabled(),
                                                            enable,
                                                            _settings->ruleTimerInterval(),
                                                            _settings->geometry());
}

void settingsManager::setTimerEnabled(bool enable)
{
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(_settings->isCloseOnExitEnabled(),
                                                            enable,
                                                            _settings->isRulesEnabled(),
                                                            _settings->ruleTimerInterval(),
                                                            _settings->geometry());
}

void settingsManager::setTimerInterval(int msec)
{
    _settings = SettingsDelegateBuilder::buildSettingsDelegate(_settings->isCloseOnExitEnabled(),
                                                            _settings->isRuleTimerEnabled(),
                                                            _settings->isRulesEnabled(),
                                                            msec,
                                                            _settings->geometry());
}

const ISettingsModel *settingsManager::settingsState() const
{
    return _settings;
}

void settingsManager::setSettings(const ISettingsModel *s)
{
    _settings = s;
}
