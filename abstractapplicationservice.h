#ifndef ABSTRACTCOREAPPLICATION_H
#define ABSTRACTCOREAPPLICATION_H

#include <qstring.h>
#include "icon.h"
#include "abstractrulesmanager.h"
#include "isettingsdelegate.h"
#include "imutableobject.h"
#include "abstractsettingsmanager.h"
#include "abstractqueueManager.h"
#include "abstractfileinformationmanager.h"
#include "ithreadmanagerinterface.h"
#include "abstractfilesystemwatcher.h"
#include "AbstractFileWorker.h"
#include "iruledefinitions.h"
#include "ifiltereringcontext.h"

typedef IFiltereringContext<IRule<IRuleCondition<SizeLimit, SizeLimits, DateInterval>>,
            ITreeModelDelegate<DefaultFileModel, DefaultModelType>> DefaultFilteringContext;

class AbstractApplicationService :
        public QObject,
        public IMutableObject,
        public IBroadcastingObject
{
    Q_OBJECT
public:
    virtual ~AbstractApplicationService() = default;

    virtual QString watchFolder(int index) const = 0;
    virtual void clearWatchFolders() = 0;
    virtual int watchFolderCount() = 0;
    virtual QStringList watchFolders() = 0;
    virtual QList<QTreeWidgetItem*> watchFolderItems() = 0;
    virtual QList<QTreeWidgetItem*> detailedWatchFolderItems() = 0;

    virtual const IRule<IDefaultRuleCondition>* ruleAt(int index) = 0;
    virtual const IRule<IDefaultRuleCondition>* rule(QString title) = 0;
    virtual QList<QTreeWidgetItem*> ruleItemModels() = 0;
    virtual void swapRule(int i, int j) = 0;
    virtual void clearRules() const = 0;

    virtual QString directoryInformationHtml(QString path) = 0;

    virtual QList<QTreeWidgetItem*> suffixList(const QString &path) = 0;

    virtual QList<const AbstractIcon*> icons() = 0;

    virtual const ISettingsDelegate* settingsState() = 0;
    virtual void setSettings(const bool &closeOnExit,
                             const bool &ruleTimerEnabled,
                             const bool &rulesEnabled,
                             const QRect &geometry,
                             const int &countInterval) =0;

    virtual bool closeOnExit() = 0;

    virtual void calculateFolderSize(QString path) = 0;

    virtual void configureServices() = 0;

    virtual void startServices() = 0;

    // Add services

    virtual void setRuleManagerService(AbstractRulesManager* service) = 0;
    virtual void setSettingsManagerService(AbstractSettingsManager* service) = 0;
    virtual void setEntityQueueManagerService(AbstractQueueManager* service) = 0;
    virtual void setFileInformationManagerService(AbstractFileInformationManager* service) = 0;
    virtual void setThreadManagerService(IThreadManagerInterface* service) = 0;

    virtual void setFileOperationsService(AbstractFileWorker* service) = 0;
    virtual void setFileModelBuilderService(IFileListService<IModelBuilder<IFileModel<>,QString>> *service) = 0;
    virtual void setFileWatcherService(AbstractFileSystemWatcher* service) = 0;

    virtual IRuleDefinitions* RuleDefinitionsService() = 0;
    virtual void setRuleDefinitionsService(IRuleDefinitions *service) = 0;

    virtual void setFilteringContext(DefaultFilteringContext* service) = 0;

public slots:
    virtual void clearFolders(QStringList paths) = 0;
    virtual void clearFoldersAccordingToRules(QStringList paths) = 0;

    virtual void addWatchFolder(QString path) = 0;
    virtual void addWatchFolders(QStringList paths) = 0;

    virtual void removeWatchFolderAt(int index) = 0;
    virtual void removeWatchFolder(QString path) = 0;

    virtual void insertRule(const IRule<IDefaultRuleCondition>* r) = 0;
    virtual void replaceRule(const IRule<IDefaultRuleCondition>* newRule, QString title) = 0;
    virtual void removeRuleAt(int index) = 0;
    virtual void removeRule(QString title) = 0;

signals:
    // FileWorker

    void sendFolderSize(const DirectoryEntity *fObject);
    void sendStatusMessage(const QString &filePath);
    void sendEntity(IModelDelegate<EntityModel,EntityType> *delegate);

    void stateChanged();

    void sendSystemTrayMessage(const QString &title, const QString &msg);
    void sendStatusLineMessage(const QString &msg);
};

#endif // ABSTRACTCOREAPPLICATION_H
