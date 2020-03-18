#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

//#define TEST_MODE

#include "abstractapplicationservice.h"
#include "settingsdelegate.h"

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

    const IRule<>* ruleAt(int index) override {return rulesService->rule(index);}
    const IRule<>* rule(QString title) override {return rulesService->rule(title);}
    QList<QTreeWidgetItem*> ruleItemModels() override {return rulesService->ruleItems();}
    void swapRule(int i, int j) override {rulesService->swapRule(i,j);}
    void clearRules() const override ;

    void insertRule(const IRule<>* r) override {rulesService->addRule(r);}
    void replaceRule(const IRule<>* newRule, QString title) override {rulesService->replaceRule(newRule,title);}
    void removeRuleAt(int index) override {rulesService->removeRuleAt(index);}
    void removeRule(QString title) override {rulesService->removeRule(title);}


    // Persistence interface

    const ISettingsDelegate* settingsState() override;
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

    void configureServices()  override;
    void startServices() override;

    void setRuleManagerService(AbstractRulesManager* service) override {rulesService = service;}
    void setSettingsManagerService(AbstractSettingsManager *service) override {settingsService = service;};
    void setEntityQueueManagerService(AbstractQueueManager* service) override {queueService = service;}
    void setFileInformationManagerService(AbstractFileInformationManager *service) override {informationService = service;}
    void setThreadManagerService(IThreadManagerInterface *service) override {threadingService = service;}

    void setFileOperationsService(AbstractFileWorker *service) override {fileOperationsService = service;}
    void setFileModelBuilderService(IFileListService *service) override {fileListService = service;}
    void setFileWatcherService(AbstractFileSystemWatcher *service) override {fileWatcherService = service;}

    IDefinitions * RuleDefinitionsService() override
    {
        return _ruleDefinitionsService;
    }
    void setRuleDefinitionsService(IDefinitions *service) override
    {
        _ruleDefinitionsService = service;
    }

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
    IFileListService *fileListService;

    AbstractSettingsManager *settingsService;
    IThreadManagerInterface *threadingService;
    IDefinitions *_ruleDefinitionsService;
};

#endif // MAINAPPLICATION_H
