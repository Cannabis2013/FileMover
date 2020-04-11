#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#define TEST_MODE

#include "abstractapplicationservice.h"
#include "settingsdelegate.h"
#include "imodelbuilder.h"

class ApplicationDomain : public AbstractApplicationService
{
public:
    ApplicationDomain();

    ~ApplicationDomain();

    QString directoryInformationHtml(QString path) override {return informationService->createTextBrowserHtml(path);}


    // Watchfolder and detailed folder interface
    void addWatchFolders(QStringList paths) override;
    void clearWatchFolders() override;
    QString watchFolder(int index) const override;
    QStringList watchFolders() override;
    QList<QTreeWidgetItem *> watchFolderItems() override {return settingsService->pathItems();}
    QList<QTreeWidgetItem*> detailedWatchFolderItems() override {return informationService->allTreeItems();}
    int watchFolderCount() override;

    QList<QTreeWidgetItem*> suffixList(const QString &path) override {return informationService->suffixList(path);}

    /*
     * Rules interface
     */

    const IRule<IDefaultRuleCondition>* ruleAt(int index) override {return rulesService->rule(index);}
    const IRule<IDefaultRuleCondition>* rule(const QString &title) override {return rulesService->rule(title);}
    QList<QTreeWidgetItem*> ruleItemModels() override {return rulesService->ruleItems();}
    void swapRule(int i, int j) override {rulesService->swapRule(i,j);}
    void clearRules() const override ;

    void insertRule(const IRule<IDefaultRuleCondition>* r) override {rulesService->addRule(r);}
    void replaceRule(const IRule<IDefaultRuleCondition>* newRule, QString title) override {rulesService->replaceRule(newRule,title);}
    void removeRuleAt(int index) override {rulesService->removeRuleAt(index);}
    void removeRule(QString title) override {rulesService->removeRule(title);}


    // Persistence interface

    const ISettingsModel* settingsState() override;
    void setSettings(const bool &closeOnExit,
                     const bool &ruleTimerEnabled,
                     const bool &rulesEnabled,
                     const QRect &geometry,
                     const int &countInterval) override;

    // Basic settings interface
    bool closeOnExit() override {return settingsService->closeOnQuit();}
    QList<const AbstractIcon*> icons() override {return settingsService->allIcons();}

    // File/folder operations

    void calculateFolderSize(QString path)  override;

    void clearFolders(QStringList paths)  override;
    void clearFoldersAccordingToRules(QStringList paths)  override;

    AbstractApplicationService* configureServices()  override;
    AbstractApplicationService* startServices() override;

    AbstractApplicationService* setRuleManagerService(AbstractRulesManager* service) override;
    AbstractApplicationService* setSettingsManagerService(AbstractSettingsManager *service) override;
    AbstractApplicationService* setEntityQueueManagerService(AbstractQueueManager* service) override;
    AbstractApplicationService* setFileInformationManagerService(AbstractFileInformationManager *service) override;
    AbstractApplicationService* setThreadManagerService(IThreadManagerInterface *service) override;

    AbstractApplicationService* setFileOperationsService(AbstractFileWorker *service) override;
    AbstractApplicationService* setFileModelBuilderService(IFileListService<IModelBuilder<IFileModel<>,QString>> *service) override;
    AbstractApplicationService* setFileWatcherService(AbstractFileSystemWatcher *service) override;

    IRuleDefinitions * RuleDefinitionsService() override {return _ruleDefinitionsService;}
    AbstractApplicationService* setRuleDefinitionsService(IRuleDefinitions *service) override;

    AbstractApplicationService* setFilteringContext(IDefaultFilteringContext* filterService, DefaulFileList* listService) override;

    void addWatchFolder(QString path) override {settingsService->insertPath(path);}
    void removeWatchFolderAt(int index) override;
    void removeWatchFolder(QString path) override;


private:
    AbstractFileWorker *fileOperationsService;
    AbstractFileSystemWatcher *fileWatcherService;

    // Manager interfaces
    AbstractRulesManager *rulesService;
    AbstractQueueManager *queueService;
    AbstractFileInformationManager *informationService;
    IFileListService<IModelBuilder<IFileModel<>,QString>> *fileListService;

    AbstractSettingsManager *settingsService;
    IThreadManagerInterface *threadingService;
    IRuleDefinitions *_ruleDefinitionsService;
    IDefaultFilteringContext *filteringService;
};

#endif // MAINAPPLICATION_H
